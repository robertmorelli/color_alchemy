const chokidar = require(`chokidar`);
const { exec } = require(`child_process`);
const path = require(`path`);
const fs = require(`fs`);
const cheerio = require(`cheerio`);
const src = `artifacts/index.html`;
const out = `docs/index.html`;
const mime_map = new Map([
  [`.js`, `application/javascript`],
  [`.jpg`, `image/jpeg`],
  [`.jpeg`, `image/jpeg`],
  [`.png`, `image/png`],
  [`.gif`, `image/gif`],
  [`.svg`, `image/svg+xml`],
  [`.css`, `text/css`],
  [`.js`, `application/javascript`],
  [`.json`, `application/json`],
  [`.woff`, `font/woff`],
  [`.woff2`, `font/woff2`],
  [`.ttf`, `font/ttf`],
  [`.otf`, `font/otf`],
  [`.eot`, `application/vnd.ms-fontobject`],
]);

function rmrf(target) {
  if (!fs.existsSync(target)) return;
  if (fs.statSync(target).isDirectory()) {
    for (const file of fs.readdirSync(target)) {
      rmrf(path.resolve(target, file));
    }
    fs.rmdirSync(target);
  } else {
    fs.unlinkSync(target);
  }
}

const nuke = () => {
  sassProcess?.kill();
  tsProcess?.kill();
  docWatcher?.close();
  inlineWatcher?.close();
  if (!fs.existsSync(`artifacts`)) {
    log_action(`Watcher`, `nuke`, `Keyboard interrupt received, no artifacts to nuke...`);
    return process.exit(0);
  }
  rmrf(`artifacts`);
  log_action(`Watcher`, `nuke`, `Keyboard interrupt received, nuking...`);
  return process.exit(0);
};

const inline_attribute = (attribute) =>
  ($) =>
    (element) => {
      $(element).each((_, el) => {
        const attr_value = $(el).attr(attribute);
        const abs = path.resolve(path.dirname(src), attr_value);
        if (fs.existsSync(abs)) {
          log_action(`Inline`, `embed`, `Embedding (${attribute}): ${abs}`);
          const asset = fs.readFileSync(abs);
          const mime_type_value = mime_type(abs);
          const base64Data = asset.toString(`base64`);
          const dataUri = `data:${mime_type_value};base64,${base64Data}`;
          $(el).attr(attribute, dataUri);
          log_action(`Inline`, `converted`, `Asset (${attribute}) to data URI: ${abs}`);
        } else {
          log_action(`Inline`, `warn`, `Asset (${attribute}) file not found: ${abs}`);
        }
      });
    };

let print_counter = 0;
const log_action = (location, action, msg) => {
  print_counter += 1;
  const seg_length = 15;
  const counter_raw = `${print_counter}`;
  const counter_padded = counter_raw.padStart(seg_length, ` `);
  const location_raw = `${location}`;
  const location_string = `[${location_raw}]`;
  const location_padded = location_string.padEnd(seg_length, ` `);
  const action_raw = `${action}`;
  const action_string = `<${action_raw}>`;
  const action_padded = action_string.padEnd(seg_length, ` `);
  const msg_string = `| ${msg}`;
  const combine = [counter_padded, location_padded, action_padded, msg_string];
  console.log(combine.join(` `));
};

const log_status = (location, action, succ_msg, err, stdout, stderr) => {
  if (err) log_action(location, action, err);
  if (stdout) log_action(location, `stdout`, stdout.trim());
  if (stderr) log_action(location, `stderr`, stderr.trim());
  log_action(location, `continue`, succ_msg);
}


const mime_type = (filePath) => {
  const ext = path.extname(filePath).toLowerCase();
  return mime_map.get(ext) || `application/octet-stream`;
};

const compile_typst = async (fileName) => {
  const base = path.basename(fileName, `.typ`);
  const outSvg = path.join(`artifacts/documents`, `${base}.svg`);
  const command = `typst compile --format svg "${fileName}" "${outSvg}"`;
  log_action(`Documents`, `run`, fileName);
  exec(command, { maxBuffer: 1024 * 1024 }, (err, stdout, stderr) => {
    log_status(`Documents`, `done`, `${fileName} → ${outSvg}`, err, stdout, stderr);
  });
};

const generate_other_artifacts = async () => {
  fs.copyFileSync(`src/index.html`, `artifacts/index.html`);
  //TODO: move js files and create headers for all assets
};

const inline_artifacts = async () => {
  const html = fs.readFileSync(src, `utf8`);
  const $ = cheerio.load(html);
  [`link[href][rel="preload"]`, `link[href][rel="stylesheet"]`].forEach(inline_attribute(`href`)($));
  [`link[src][rel="preload"]`].forEach(inline_attribute(`src`)($));
  [`script[src]`].forEach(inline_attribute(`src`)($));
  if (!fs.existsSync(path.dirname(out))) {
    fs.mkdirSync(path.dirname(out), { recursive: true });
    log_action(`Inline`, `created`, `${path.dirname(out)}`);
  }
  fs.writeFileSync(out, $.html());
  log_action(`Inline`, `done`, `${out}`);
};

let sassProcess;
let tsProcess;
let docWatcher;
let inlineWatcher;

try {
  [`artifacts`, `artifacts/documents`, `artifacts/scripts`, `artifacts/styles`, `docs`,].forEach(dir => {
    if (!fs.existsSync(dir)) {
      fs.mkdirSync(dir, { recursive: true });
      log_action(`Watcher`, `created`, `directory: ${dir}`);
    } else {
      log_action(`Watcher`, `skip`, `directory exists: ${dir}`);
    }
  });

  setTimeout(() => {
    sassProcess = exec(`npx sass --no-source-map --watch src/styles:artifacts/styles`);
    sassProcess.on(`error`, error => log_action(`Styles`, `error`, error));
    sassProcess.stdout?.on(`data`, data => log_action(`Styles`, `stdout`, data));
    sassProcess.stderr?.on(`data`, data => log_action(`Styles`, `stderr`, data));

    tsProcess = exec(`npx tsc --strict --removeComments --watch`);
    tsProcess.on(`error`, error => log_action(`TScript`, `error`, error));
    tsProcess.stdout?.on(`data`, data => log_action(`TScript`, `stdout`, data));
    tsProcess.stderr?.on(`data`, data => log_action(`TScript`, `stderr`, data));

    docWatcher = chokidar.watch(`src/documents/`, { ignoreInitial: false })
      .on(`add`, compile_typst)
      .on(`change`, compile_typst)
      .on(`error`, error => log_action(`Documents`, `error`, error))
      .on(`ready`, () => log_action(`Documents`, `ready`, `src/documents/`));

    setTimeout(() => {
      inlineWatcher = chokidar.watch([
        `src/`,
      ], { ignoreInitial: false })
        .on(`add`, generate_other_artifacts)
        .on(`change`, generate_other_artifacts)
        .on(`error`, error => log_action(`Stage One`, `error`, error))
        .on(`ready`, () => log_action(`Stage One`, `ready`, `src/`));
      log_action(`Stage One`, `initialized`, `src/`);
    }, 300);
    setTimeout(() => {
      inlineWatcher = chokidar.watch([
        `artifacts/`,
      ], { ignoreInitial: false })
        .on(`add`, inline_artifacts)
        .on(`change`, inline_artifacts)
        .on(`error`, error => log_action(`Stage Two`, `error`, error))
        .on(`ready`, () => log_action(`Stage Two`, `ready`, `artifacts/`));
      log_action(`Stage Two`, `initialized`, `artifacts/`);
    }, 500);
  }, 200);
} catch (error) {
  log_action(`Watcher`, `death`, error);
  nuke();
}

// handle Ctrl+C
process.on(`SIGINT`, () => {
  log_action(`Watcher`, `interrupt`, `Keyboard interrupt received, nuking...`);
  nuke();
});