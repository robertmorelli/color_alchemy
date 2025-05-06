const chokidar = require(`chokidar`);
const { exec } = require(`child_process`);
const path = require(`path`);
const fs = require(`fs`);
const cheerio = require(`cheerio`);

const Typst = async (fileName) => {
  const base = path.basename(fileName, `.typ`);
  const outSvg = path.join(`documents_mid_compile`, `${base}.svg`);
  const cmd = `typst compile --format svg "${fileName}" "${outSvg}"`;
  console.log(`[Documents]\t\t<run> ${cmd}`);
  exec(cmd, { maxBuffer: 1024 * 1024 }, (err, stdout, stderr) => {
    if (err) return void console.error(`[Documents]\t\t<error>`, err);
    if (stdout) console.log(`[Documents]\t\t<stdout> ${stdout.trim()}`);
    if (stderr) console.warn(`[Documents]\t\t<stderr> ${stderr.trim()}`);
    console.log(`[Documents]\t\t<done> ${fileName} → ${outSvg}`);
  });
};

const Inline = async () => {
  const cmd = `npx html-inline-external --src src/index.html --dest index_staging/index.html --pretty`;
  console.log(`[Inline]\t\t<run> ${cmd}`);
  exec(cmd, (err, stdout, stderr) => {
    if (err) return void console.error(`[Inline]\t\t<error>`, err);
    if (stdout) console.log(`[Inline]\t\t<stdout> ${stdout.trim()}`);
    if (stderr) console.warn(`[Inline]\t\t<stderr> ${stderr.trim()}`);
    const src = `index_staging/index.html`;
    const out = `docs/index.html`;
    if (!fs.existsSync(src)) return void console.warn(`[Inline]\t\t<skip> source not found (${src})`);
    console.log(`[Inline]\t\t<read> ${src}`);
    setTimeout(() => {
      const html = fs.readFileSync(src, `utf8`);
      const $ = cheerio.load(html);
      [`object[type="image/svg+xml"]`, `img[src$=".svg"]`, `embed[src$=".svg"]`]
        .forEach(sel => {
          $(sel).each((_, el) => {
            const attr = $(el).attr(`data`) || $(el).attr(`src`);
            const abs = path.resolve(path.dirname(src), attr);
            if (fs.existsSync(abs)) {
              console.log(`[Inline]\t\t<embed> Embedding SVG: ${abs}`);
              const svg = fs.readFileSync(abs, `utf8`);
              $(el).replaceWith(svg);
            } else {
              console.warn(`[Inline]\t\t<warn> SVG file not found: ${abs}`);
            }
          });
        });
      if (!fs.existsSync(path.dirname(out))) {
        fs.mkdirSync(path.dirname(out), { recursive: true });
        console.log(`[Inline]\t\t<created> ${path.dirname(out)}`);
      }
      fs.writeFileSync(out, $.html());
      console.log(`[Inline]\t\t<done> ${out}`);
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
      console.log(`[Watcher]\t\t<created> directory: ${dir}`);
    } else {
      console.log(`[Watcher]\t\t<skip> directory exists: ${dir}`);
    }
  });

  setTimeout(() => {
    sassProcess = exec(`npx sass --no-source-map --watch src/styles:sass_mid_compile`);
    sassProcess.on(`error`, e => console.error(`[Styles]\t\t<error>`, e));
    sassProcess.stdout?.on(`data`, d => console.log(`[Styles]\t\t<stdout> ${d}`));
    sassProcess.stderr?.on(`data`, d => console.warn(`[Styles]\t\t<stderr> ${d}`));

    tsProcess = exec(`npx tsc --strict --removeComments --watch`);
    tsProcess.on(`error`, e => console.error(`[TScript]\t\t<error>`, e));
    tsProcess.stdout?.on(`data`, d => console.log(`[TScript]\t\t<stdout> ${d}`));
    tsProcess.stderr?.on(`data`, d => console.warn(`[TScript]\t\t<stderr> ${d}`));

    docWatcher = chokidar.watch(`src/documents/`, { ignoreInitial: false })
      .on(`add`, Typst)
      .on(`change`, Typst)
      .on(`error`, e => console.error(`[Documents]\t\t<error>`, e))
      .on(`ready`, () => console.log(`[Documents]\t\t<ready>`));

    setTimeout(() => {
      inlineWatcher = chokidar.watch([
        `sass_mid_compile/`,
        `ts_mid_compile/`,
        `documents_mid_compile/`,
        `src/index.html`,
      ], { ignoreInitial: true })
        .on(`add`, Inline)
        .on(`change`, Inline)
        .on(`error`, e => console.error(`[Inline]\t\t<error>`, e))
        .on(`ready`, () => console.log(`[Inline]\t\t<ready>`));
      console.log(`[Watcher]\t\t<ready>`);
    }, 500);
  }, 200);
} catch (e) {
  console.error(`[Watcher]\t\t<death>`, e);
  sassProcess?.kill();
  tsProcess?.kill();
  const cmd = `npm run nuke`;
  console.log(`[Watcher]\t\t<nuke> ${cmd}`);
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