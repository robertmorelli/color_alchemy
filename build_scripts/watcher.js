const chokidar = require('chokidar');
const { exec } = require('child_process');
const path = require('path');
const fs = require('fs');
const cheerio = require('cheerio');
const compileTyp = async (fileName, _) => {
  console.log(`compileTyp: ${fileName}`);
  exec(`typst compile --format svg "${fileName}" "${path.join('documents_mid_compile', `${path.basename(fileName, '.typ')}.svg`)}"`, { maxBuffer: 1024 * 1024 }, (_, __, ___) => {});
};
const inlineAll = async (fileName, _) => {
  console.log(`inlineAll: ${fileName}`);
  exec('npx html-inline-external --src src/index.html --dest index_staging/index.html --pretty', (_, __, ___) => {
    const src = 'index_staging/index.html';
    const out = 'docs/index.html';
    if (!fs.existsSync(src)) return;
    const htmlDir = path.dirname(src);
    const html = fs.readFileSync(src, 'utf8');
    const $ = cheerio.load(html);
    ['object[type="image/svg+xml"]', 'img[src$=".svg"]', 'embed[src$=".svg"]']
      .forEach(sel => {
        $(sel).each((_, el) => {
          const attr = $(el).attr('data') || $(el).attr('src');
          const abs  = path.resolve(htmlDir, attr);
          if (fs.existsSync(abs)) {
            const svg = fs.readFileSync(abs, 'utf8');
            $(el).replaceWith(svg);
          } else {
            console.warn(`WARN: SVG file not found: ${abs}`);
          }
        });
      });

    if (!fs.existsSync(path.dirname(out))) {
      fs.mkdirSync(path.dirname(out), { recursive: true });
    }
    fs.writeFileSync(out, $.html());
  });
};

let sass;
let ts;
let typ;
try {
    ['documents_mid_compile', 'docs', 'index_staging'].forEach(dir => { if (!fs.existsSync(dir)) fs.mkdirSync(dir, { recursive: true }); });
    sass = exec('npx sass src/sass_mid_compile --no-source-map --watch');
    ts = exec('npx tsc --strict --removeComments --watch');
    chokidar.watch([
      'src/documents/'
    ]).on('add', compileTyp)
      .on('change', compileTyp);
  chokidar.watch([
      'sass_mid_compile/',
      'ts_mid_compile/',
      'documents_mid_compile/',
      'src/index.html',
  ]).on('add', inlineAll)
    .on('change', inlineAll);
} catch (e) {
    if (sass) sass.kill();
    if (ts)   ts.kill();
    if (typ)  typ.kill();
}