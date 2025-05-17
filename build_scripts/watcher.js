/** Watcher for files to compile and inline
 * expects dir structure:
 * src/
 *   index.html
 *   scripts/
 *     ** / *.ts
 *     ** / *.js
 *   styles/
 *     ** / *.scss
 *   documents/
 *     ** / *.typ
 *   assets/
 *     images/
 *       ** / *.png
 *       ** / *.jpg
 *       ** / *.jpeg
 *       ** / *.gif
 *       ** / *.svg
 *     sounds/
 *       ** / *.wav
 * artifacts/
 *   index.html
 *   scripts/
 *     ** / *.js
 *   styles/
 *     ** / *.css
 *   documents/
 *     ** / *.svg
 * docs/
 *   index.html
 * 
 */


const chokidar = require(`chokidar`);
const { exec } = require(`child_process`);
const path = require(`path`);
const fs = require(`fs`);
const cheerio = require(`cheerio`);
const src = `src/index.html`;
const art = `artifacts/index.html`;
const doc = `docs/index.html`;
let print_counter = 0;
let stage_one_debounce = false;
let stage_two_debounce = false;

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
  [`.wav`, `audio/wav`],
]);

Reset = `\x1b[0m`
Bright = `\x1b[1m`
Dim = `\x1b[2m`
Underscore = `\x1b[4m`
Blink = `\x1b[5m`
Reverse = `\x1b[7m`
Hidden = `\x1b[8m`

FgBlack = `\x1b[30m`
FgRed = `\x1b[31m`
FgGreen = `\x1b[32m`
FgYellow = `\x1b[33m`
FgBlue = `\x1b[34m`
FgMagenta = `\x1b[35m`
FgCyan = `\x1b[36m`
FgWhite = `\x1b[37m`
FgGray = `\x1b[90m`

BgBlack = `\x1b[40m`
BgRed = `\x1b[41m`
BgGreen = `\x1b[42m`
BgYellow = `\x1b[43m`
BgBlue = `\x1b[44m`
BgMagenta = `\x1b[45m`
BgCyan = `\x1b[46m`
BgWhite = `\x1b[47m`
BgGray = `\x1b[100m`

const color_map = new Map([
  [`error`, BgRed + FgWhite],
  [`continue`, Dim + FgGreen],
  [`skip`, Dim + FgYellow],
  [`write`, Dim + FgGreen],
  [`default`, Reset],
  [`stderr`, BgWhite + FgRed],
  [`stdout`, Dim + FgMagenta],
  [`embed`, Dim + FgBlue],
  [`read`, Dim + FgBlue],
  [`killing`, Dim + FgMagenta],
  [`killed`, Dim + FgMagenta],
  [`ready`, Dim + FgGreen],
  [`begin`, Dim + FgGreen],
  [`done`, Dim + FgGreen],
  [`nuke`, Bright + BgMagenta + FgWhite],
  [`interrupt`, Dim + BgCyan + FgWhite],
]);

const log_action = (location, action, msg) => {
  print_counter += 1;
  const seg_length = 17;
  const counter_raw = `${print_counter}`;
  const counter_padded = counter_raw.padStart(seg_length, ` `);
  const location_raw = `${location}`;
  const location_string = `[${location_raw}]`;
  const location_padded = location_string.padEnd(seg_length, ` `);
  const action_raw = `${action}`;
  const action_color = color_map.get(action_raw) || color_map.get(`default`);
  const action_string = `${action_raw}`;
  const action_padded = `${action_color}${action_string.padEnd(seg_length, ` `)}${Reset}`;
  const msg_string = `| ${msg}`;
  const combine = [counter_padded, location_padded, action_padded, msg_string];
  console.log(combine.join(` `));
};

const inline_attribute =
  (is_stage_one) =>
    (attribute) =>
      ($) =>
        (element) => {
          $(element).each((_, el) => {
            const attr_value = `${$(el).attr(attribute)}`;
            if (attr_value.startsWith(`data:`)) {
              log_action(`Inline`, `skip`, `Asset (${attribute}) already data URI`);
              return;
            }
            const abs = path.resolve(path.dirname(art), attr_value);
            if (fs.existsSync(abs)) {
              log_action(`Inline`, `embed`, `Embedding (${attribute}): ${abs}`);
              const asset = fs.readFileSync(abs);
              const ext = path.extname(abs).toLowerCase();
              const mime_type_value = mime_map.get(ext) || `application/octet-stream`;
              const base64Data = asset.toString(`base64`);
              const dataUri = `data:${mime_type_value};base64,${base64Data}`;
              $(el).attr(attribute, dataUri);
              log_action(`Inline`, `continue`, `Asset (${attribute}) to data URI: ${abs}`);
            } else if (is_stage_one) log_action(`Inline`, `continue`, `Asset (${attribute}) handled in stage two`);
            else log_action(`Inline`, `error`, `Asset (${attribute}) file not found: ${abs}`);
          })
        };
const inline_href_stage_one = inline_attribute(true)(`href`);
const inline_src_stage_one = inline_attribute(true)(`src`);
const inline_href_stage_two = inline_attribute(false)(`href`);
const inline_src_stage_two = inline_attribute(false)(`src`);

function rmrf(target) {
  if (!fs.existsSync(target)) return;
  if (fs.statSync(target).isDirectory()) {
    for (const file of fs.readdirSync(target))
      rmrf(path.resolve(target, file));
    fs.rmdirSync(target);
  } else fs.unlinkSync(target);
}

const shutdown = () => {
  for (const [name, kill] of kill_watchers) {
    log_action(`Shutdown`, `nuke`, `${name} watcher`);
    kill();
    log_action(`Shutdown`, `nuke`, `${name} watcher destroyed`);
  }
  if (!fs.existsSync(`artifacts`)) {
    log_action(`Shutdown`, `nuke`, `No artifacts to nuke, exiting...`);
    return process.exit(0);
  }
  log_action(`Shutdown`, `nuke`, `Found artifacts, nuking`);
  rmrf(`artifacts`);
  log_action(`Shutdown`, `nuke`, `Artifacts nuked, exiting...`);
  return process.exit(0);
};

const log_status = (location, action, succ_msg, err, stdout, stderr) => {
  if (err) log_action(location, action, err);
  if (stdout) log_action(location, `stdout`, stdout.trim());
  if (stderr) log_action(location, `stderr`, stderr.trim());
  log_action(location, `continue`, succ_msg);
}

const compile_typst = async (fileName) => {
  const base = path.basename(fileName, `.typ`);
  const outSvg = path.join(`artifacts/documents`, `${base}.svg`);
  const command = `typst compile --format svg "${fileName}" "${outSvg}"`;
  log_action(`Documents`, `run`, fileName);
  exec(command, { maxBuffer: 1024 * 1024 }, (err, stdout, stderr) => {
    log_status(`Documents`, `done`, `${fileName} → ${outSvg}`, err, stdout, stderr);
  });
};
const stage_one = async (fileName) => {
  if (stage_one_debounce) return;
  stage_one_debounce = true;
  await new Promise(resolve => setTimeout(resolve, 500));
  log_action(`Stage One`, `begin`, fileName);

  // read source HTML
  const html = fs.readFileSync(src, `utf8`);
  const $ = cheerio.load(html);

  // helper to walk directories
  const walk = (dir, list = []) => {
    fs.readdirSync(dir).forEach(fn => {
      const p = path.join(dir, fn);
      if (fs.statSync(p).isDirectory()) walk(p, list);
      else list.push(p);
    });
    return list;
  };

  // -- styles (compiled CSS from Sass)
  $(`head`).append(`<!--styles-->`);
  const style_src_dir = path.join(`src`, `styles`);
  walk(style_src_dir)
    .filter(f => /\.(scss|sass)$/.test(f))
    .forEach(f => {
      const rel = path.relative(style_src_dir, f).replace(/\\/g, `/`);
      const css_file = rel.replace(/\.(scss|sass)$/, `.css`);
      const href = `../artifacts/styles/${css_file}`;
      $(`head`).append(`<link defer href="${href}" rel="stylesheet" />`);
      log_action(`Stage One`, `write`, `Style link: ${href}`);
    });

  // -- assets/documents
  $(`head`).append(`<!--assets/documents-->`);
  // raw image assets
  const img_src_dir = path.join(`src`, `assets`, `images`);
  walk(img_src_dir)
    .filter(f => /\.(png|jpe?g|gif|svg)$/.test(f))
    .forEach(f => {
      const ext = path.extname(f).toLowerCase();
      const mime = mime_map.get(ext) || `application/octet-stream`;
      const base = path.basename(f, ext);
      const safe_name = base.replace(/\./g, `_`).replace(/_/g, `-`);
      const id = `--${safe_name}`;
      const href = `../src/assets/images/${path.basename(f)}`;
      $(`head`).append(
        `<link defer href="${href}" id="${id}" rel="preload" as="image" type="${mime}" />`
      );
      log_action(`Stage One`, `write`, `Asset link: ${href}`);
    });
  // compiled document SVGs
  const doc_dir = path.join(`artifacts`, `documents`);
  walk(doc_dir)
    .filter(f => f.endsWith(`.typ`))
    .forEach(f => {
      const base_name = path.basename(f, `.typ`);
      const safe_name = base.replace(/\./g, `_`).replace(/_/g, `-`);
      const id = `--${safe_name}`;
      const href = `../artifacts/documents/${base_name}.svg`;
      $(`head`).append(
        `<link defer href="${href}" id="${id}" rel="preload" as="image" type="image/svg+xml" />`
      );
      log_action(`Stage One`, `write`, `Doc link: ${href}`);
    });

  // -- scripts (compiled JS from TS)
  $(`head`).append(`<!--scripts-->`);
  const script_src_dir = path.join(`src`, `scripts`);
  walk(script_src_dir)
    .filter(f => /\.(js|ts)$/.test(f))
    .forEach(f => {
      let rel_path = path.relative(script_src_dir, f).replace(/\\/g, `/`);
      let js_file = rel_path.replace(/\.ts$/, `.js`);
      const was_js_already = rel_path === js_file;
      const folder = was_js_already ? `src` : `artifacts`;
      const href = `../${folder}/scripts/${js_file}`;
      const base = path.basename(f, path.extname(f));
      let type_attr = `text/javascript`;
      let id_attr = ``;
      if (/\.worker\.(js|ts)$/.test(f)) {
        type_attr = `text/worker`;
        const safe_name = base.replace(/\./g, `_`).replace(/_/g, `-`);
        id_attr = ` id="${safe_name}"`;
      } else if (/handler\.(js|ts)$/.test(f)) {
        type_attr = `module`;
      }
      $(`head`).append(
        `<script defer src="${href}"${id_attr} type="${type_attr}"></script>`
      );
      log_action(`Stage One`, `write`, `Script link: ${href}`);
    });

  // inline assets
  log_action(`Stage One`, `read`, src);
  [`link[href][rel="preload"]`, `link[href][rel="stylesheet"]`].forEach(inline_href_stage_one($));
  [`link[src][rel="preload"]`, `script[src]`].forEach(inline_src_stage_one($));

  // write out
  fs.writeFileSync(art, $.html());
  log_action(`Stage One`, `done`, art);
  stage_one_debounce = false;
};

const stage_two = async (fileName) => {
  if (stage_two_debounce) return;
  stage_two_debounce = true;
  await new Promise(resolve => void setTimeout(resolve, 500));
  log_action(`Stage Two`, `begin`, `${fileName}`);
  const html = fs.readFileSync(art, `utf8`);
  const $ = cheerio.load(html);
  log_action(`Stage Two`, `read`, art);
  [`link[href][rel="preload"]`, `link[href][rel="stylesheet"]`].forEach(inline_href_stage_two($));
  [`link[src][rel="preload"]`, `script[src]`].forEach(inline_src_stage_two($));
  fs.writeFileSync(doc, $.html());
  log_action(`Stage Two`, `done`, doc);
  stage_two_debounce = false;
};

const kill_watchers = new Map();

const dir_init = () => {
  [`artifacts`, `artifacts/documents`, `artifacts/scripts`, `artifacts/styles`, `docs`,].forEach(dir => {
    if (!fs.existsSync(dir)) {
      fs.mkdirSync(dir, { recursive: true });
      log_action(`Watcher`, `write`, `directory: ${dir}`);
    } else {
      log_action(`Watcher`, `skip`, `directory exists: ${dir}`);
    }
  });
};

const start_external_watcher = (name, command) => {
  const proc = exec(command, { maxBuffer: 1024 * 1024 });
  proc.on(`error`, (error) => log_action(name, `error`, error));
  proc.stdout?.on(`data`, (data) => log_action(name, `stdout`, data));
  proc.stderr?.on(`data`, (data) => log_action(name, `stderr`, data));
  return proc;
};

const startup = async () => {
  dir_init();
  const style_proc = start_external_watcher(`Styles`, `npx sass --no-source-map --watch src/styles:artifacts/styles`);
  kill_watchers.set(`Styles`, () => {
    style_proc.kill();
  });
  log_action(`Startup`, `begin`, `Styles`);
  const doc_proc = start_external_watcher(`TScript`, `npx tsc --strict --removeComments --watch`);
  kill_watchers.set(`TScript`, () => { doc_proc.kill(); });
  log_action(`Startup`, `begin`, `TScript`);

  let doc_resolver;
  const doc_init = new Promise((resolve) => void (doc_resolver = resolve));
  const doc_watcher = chokidar.watch(
    [`src/documents/`], { ignoreInitial: false })
    .on(`add`, compile_typst)
    .on(`change`, compile_typst)
    .on(`error`, (error) => log_action(`Documents`, `error`, error))
    .on(`ready`, () => doc_resolver());
  kill_watchers.set(`Documents`, () => { doc_watcher.close(); });
  log_action(`Startup`, `begin`, `Documents`);
  await doc_init;
  log_action(`Startup`, `ready`, `Documents`);

  let stage_one_resolver;
  const stage_one_init = new Promise((resolve) => void (stage_one_resolver = resolve));
  const stage_one_watcher = chokidar.watch(
    [`src/index.html`, `src/assets/`, `src/scripts/`], { ignoreInitial: false })
    .on(`add`, stage_one)
    .on(`change`, stage_one)
    .on(`error`, (error) => log_action(`Stage One`, `error`, error))
    .on(`ready`, () => stage_one_resolver());
  kill_watchers.set(`Stage One`, () => { stage_one_watcher.close(); });
  log_action(`Startup`, `begin`, `Stage One`);
  await stage_one_init;
  log_action(`Startup`, `ready`, `Stage One`);

  let stage_two_resolver;
  const stage_two_init = new Promise((resolve) => void (stage_two_resolver = resolve));
  const stage_two_watcher = chokidar.watch(
    [`artifacts/`], { ignoreInitial: false })
    .on(`add`, stage_two)
    .on(`change`, stage_two)
    .on(`error`, (error) => log_action(`Stage Two`, `error`, error))
    .on(`ready`, () => stage_two_resolver());
  kill_watchers.set(`Stage Two`, () => { stage_two_watcher.close(); });
  log_action(`Startup`, `begin`, `Stage Two`);
  await stage_two_init;
  log_action(`Startup`, `ready`, `Stage Two`);
  log_action(`Startup`, `done`, `Watchers ready`);
};

startup().catch((error) => {
  log_action(`Watcher`, `error`, error);
  shutdown();
});

// handle Ctrl+C
process.on(`SIGINT`, () => {
  console.log(`\n`);
  log_action(`Watcher`, `interrupt`, `Keyboard interrupt received, nuking`);
  shutdown();
});