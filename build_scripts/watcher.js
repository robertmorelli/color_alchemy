const chokidar = require(`chokidar`);
const { exec } = require(`child_process`);
const path = require(`path`);
const fs = require(`fs`);
const cheerio = require(`cheerio`);
let print_counter = 0;
const C = () => {
  print_counter += 1;
  return String(print_counter).padStart(6, '0');
};
const MimeType = (filePath) => {
  const ext = path.extname(filePath).toLowerCase();
  const mimeTypes = {
    '.jpg': 'image/jpeg',
    '.jpeg': 'image/jpeg',
    '.png': 'image/png',
    '.gif': 'image/gif',
    '.svg': 'image/svg+xml',
    '.css': 'text/css',
    '.js': 'application/javascript',
    '.json': 'application/json',
    '.woff': 'font/woff',
    '.woff2': 'font/woff2',
    '.ttf': 'font/ttf',
    '.otf': 'font/otf',
    '.eot': 'application/vnd.ms-fontobject'
  };
  return mimeTypes[ext] || 'application/octet-stream';
};

const Typst = async (fileName) => {
  const base = path.basename(fileName, `.typ`);
  const outSvg = path.join(`documents_mid_compile`, `${base}.svg`);
  const cmd = `typst compile --format svg "${fileName}" "${outSvg}"`;
  console.log(`[Documents](${C()})\t<run> ${cmd}`);
  exec(cmd, { maxBuffer: 1024 * 1024 }, (err, stdout, stderr) => {
    if (err) return void console.error(`[Documents](${C()})\t<error>`, err);
    if (stdout) console.log(`[Documents](${C()})\t<stdout> ${stdout.trim()}`);
    if (stderr) console.warn(`[Documents](${C()})\t<stderr> ${stderr.trim()}`);
    console.log(`[Documents](${C()})\t<done> ${fileName} → ${outSvg}`);
  });
};

const Inline = async () => {
  const cmd = `npx html-inline-external --src src/index.html --dest index_staging/index.html --pretty`;
  console.log(`[Inline](${C()})\t<run> ${cmd}`);
  exec(cmd, (err, stdout, stderr) => {
    if (err) return void console.error(`[Inline](${C()})\t<error>`, err);
    if (stdout) console.log(`[Inline](${C()})\t<stdout> ${stdout.trim()}`);
    if (stderr) console.warn(`[Inline](${C()})\t<stderr> ${stderr.trim()}`);
    const src = `index_staging/index.html`;
    const out = `docs/index.html`;
    if (!fs.existsSync(src)) return void console.warn(`[Inline](${C()})\t<skip> source not found (${src})`);
    console.log(`[Inline](${C()})\t<read> ${src}`);
    setTimeout(() => {
      const html = fs.readFileSync(src, `utf8`);
      const $ = cheerio.load(html);
      [`object[type="image/svg+xml"]`, `img[src$=".svg"]`, `embed[src$=".svg"]`]
        .forEach(sel => {
          $(sel).each((_, el) => {
            const attr = $(el).attr(`data`) || $(el).attr(`src`);
            const abs = path.resolve(path.dirname(src), attr);
            if (fs.existsSync(abs)) {
              console.log(`[Inline](${C()})\t<embed> Embedding SVG: ${abs}`);
              const svg = fs.readFileSync(abs, `utf8`);
              $(el).replaceWith(svg);
            } else {
              console.warn(`[Inline](${C()})\t<warn> SVG file not found: ${abs}`);
            }
          });
        });
      [`link[href][rel="preload"]`]
        .forEach(sel => {
          $(sel).each((_, el) => {
            const attr = $(el).attr(`href`);
            const abs = path.resolve(path.dirname(src), attr);
            if (fs.existsSync(abs)) {
              console.log(`[Inline](${C()})\t<embed> Embedding Asset: ${abs}`);
              const asset = fs.readFileSync(abs);
              const mimeType = MimeType(abs);
              const base64Data = asset.toString('base64');
              const dataUri = `data:${mimeType};base64,${base64Data}`;
              $(el).attr('href', dataUri);
              console.log(`[Inline](${C()})\t<converted> Asset to data URI: ${abs}`);
            } else {
              console.warn(`[Inline](${C()})\t<warn> Asset file not found: ${abs}`);
            }
          });
        });
      if (!fs.existsSync(path.dirname(out))) {
        fs.mkdirSync(path.dirname(out), { recursive: true });
        console.log(`[Inline](${C()})\t<created> ${path.dirname(out)}`);
      }
      fs.writeFileSync(out, $.html());
      console.log(`[Inline](${C()})\t<done> ${out}`);
    }, 50);
  });
};

let sassProcess;
let tsProcess;
let docWatcher;
let inlineWatcher;

try {
  [`documents_mid_compile`, `docs`, `index_staging`, `sass_mid_compile`].forEach(dir => {
    if (!fs.existsSync(dir)) {
      fs.mkdirSync(dir, { recursive: true });
      console.log(`[Watcher](${C()})\t<created> directory: ${dir}`);
    } else {
      console.log(`[Watcher](${C()})\t<skip> directory exists: ${dir}`);
    }
  });

  setTimeout(() => {
    sassProcess = exec(`npx sass --no-source-map --watch src/styles:sass_mid_compile`);
    sassProcess.on(`error`, e => console.error(`[Styles](${C()})\t<error>`, e));
    sassProcess.stdout?.on(`data`, d => console.log(`[Styles](${C()})\t<stdout> ${d}`));
    sassProcess.stderr?.on(`data`, d => console.warn(`[Styles](${C()})\t<stderr> ${d}`));

    tsProcess = exec(`npx tsc --strict --removeComments --watch`);
    tsProcess.on(`error`, e => console.error(`[TScript](${C()})\t<error>`, e));
    tsProcess.stdout?.on(`data`, d => console.log(`[TScript](${C()})\t<stdout> ${d}`));
    tsProcess.stderr?.on(`data`, d => console.warn(`[TScript](${C()})\t<stderr> ${d}`));

    docWatcher = chokidar.watch(`src/documents/`, { ignoreInitial: false })
      .on(`add`, Typst)
      .on(`change`, Typst)
      .on(`error`, e => console.error(`[Documents](${C()})\t<error>`, e))
      .on(`ready`, () => console.log(`[Documents](${C()})\t<ready>`));

    setTimeout(() => {
      inlineWatcher = chokidar.watch([
        `sass_mid_compile/`,
        `ts_mid_compile/`,
        `documents_mid_compile/`,
        `src/index.html`,
        `src/scripts/`,
      ], { ignoreInitial: true })
        .on(`add`, Inline)
        .on(`change`, Inline)
        .on(`error`, e => console.error(`[Inline](${C()})\t<error>`, e))
        .on(`ready`, () => console.log(`[Inline](${C()})\t<ready>`));
      console.log(`[Watcher](${C()})\t<ready>`);
    }, 500);
  }, 200);
} catch (e) {
  console.error(`[Watcher](${C()})\t<death>`, e);
  sassProcess?.kill();
  tsProcess?.kill();
  const cmd = `npm run nuke`;
  console.log(`[Watcher](${C()})\t<nuke> ${cmd}`);
  exec(cmd);
}

// handle Ctrl+C
process.on('SIGINT', () => {
  console.log(`\n[Watcher]\t<interrupt> Keyboard interrupt received, nuking...`);
  sassProcess?.kill();
  tsProcess?.kill();
  docWatcher?.close();
  inlineWatcher?.close();
  exec(`npm run nuke`, (err, stdout, stderr) => {
    if (err) console.error(`[Watcher]\t<nuke error>`, err);
    if (stdout) console.log(`[Watcher]\t<nuke stdout> ${stdout.trim()}`);
    if (stderr) console.warn(`[Watcher]\t<nuke stderr> ${stderr.trim()}`);
    process.exit(0);
  });
});