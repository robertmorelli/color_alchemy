// build_scripts/watcher.js
// Watcher: Typst → SVG → inline HTML

const chokidar = require('chokidar');
const { exec } = require('child_process');
const path = require('path');
const fs = require('fs');
const cheerio = require('cheerio');

function ensureDirs() {
  ['documents_mid_compile', 'docs'].forEach(dir => {
    if (!fs.existsSync(dir)) fs.mkdirSync(dir, { recursive: true });
  });
}

function compileTypstToSvg(srcPath) {
  const base = path.basename(srcPath, '.typ');
  const svgOut = path.join('documents_mid_compile', `${base}.svg`);

  console.log(`→ Typst: ${srcPath} → ${svgOut}`);
  exec(
    `typst compile --format svg "${srcPath}" "${svgOut}"`,
    { maxBuffer: 1024 * 1024 },
    (err, stdout, stderr) => {
      if (stdout) console.log(stdout.trim());
      if (stderr) console.error(stderr.trim());
      if (err) {
        console.error('ERROR: Typst SVG compile failed:', err);
        return;
      }
      if (!fs.existsSync(svgOut)) {
        console.error(`ERROR: SVG not created: ${svgOut}`);
        return;
      }
      console.log(`→ SVG generated: ${svgOut}`);
      inlineHTML();
    }
  );
}

function inlineHTML() {
  const src = 'src/index.html';
  const out = 'docs/index.html';
  if (!fs.existsSync(src)) {
    console.error('ERROR: src/index.html missing');
    return;
  }
  const htmlDir = path.dirname(src);
  const html = fs.readFileSync(src, 'utf8');
  const $ = cheerio.load(html);

  // inline CSS
  $('link[rel="stylesheet"]').each((_, el) => {
    const href = $(el).attr('href');
    const abs = path.resolve(htmlDir, href);
    if (fs.existsSync(abs)) {
      const css = fs.readFileSync(abs, 'utf8');
      $(el).replaceWith(`<style>\n${css}\n</style>`);
    } else {
      console.warn(`WARN: CSS file not found: ${abs}`);
    }
  });

  // inline JS
  $('script[src]').each((_, el) => {
    const srcAttr = $(el).attr('src');
    const abs = path.resolve(htmlDir, srcAttr);
    if (fs.existsSync(abs)) {
      const js = fs.readFileSync(abs, 'utf8');
      $(el).replaceWith(`<script>\n${js}\n</script>`);
    } else {
      console.warn(`WARN: JS file not found: ${abs}`);
    }
  });

  // inline SVGs
  ['object[type="image/svg+xml"]', 'img[src$=".svg"]', 'embed[src$=".svg"]'].forEach(sel => {
    $(sel).each((_, el) => {
      const attr = $(el).attr('data') || $(el).attr('src');
      const abs = path.resolve(htmlDir, attr);
      if (fs.existsSync(abs)) {
        const svg = fs.readFileSync(abs, 'utf8');
        $(el).replaceWith(svg);
      } else {
        console.warn(`WARN: SVG file not found: ${abs}`);
      }
    });
  });

  if (!fs.existsSync(path.dirname(out))) fs.mkdirSync(path.dirname(out), { recursive: true });
  fs.writeFileSync(out, $.html());
  console.log(`→ Inlined HTML → ${out}`);
}

function watchAll() {
  ensureDirs();

  // initial .typ compile
  fs.readdirSync('src/documents')
    .filter(f => f.endsWith('.typ'))
    .forEach(f => compileTypstToSvg(path.join('src/documents', f)));

  // watch .typ files
  chokidar.watch('src/documents/**/*.typ', { ignoreInitial: true })
    .on('add', compileTypstToSvg)
    .on('change', compileTypstToSvg);

  // watch SVG output and HTML/CSS/JS sources
  chokidar.watch([
    'documents_mid_compile/**/*.svg',
    'sass_mid_compile/**/*.css',
    'ts_mid_compile/**/*.js',
    'src/index.html'
  ], { awaitWriteFinish: true })
    .on('all', (event, file) => {
      console.log(`→ Event ${event}: ${file}`);
      inlineHTML();
    });

  // start Sass and TS watchers
  exec('npx sass src:sass_mid_compile --no-source-map --watch');
  exec('npx tsc --watch --strict --removeComments');

  console.log('Watching: .typ→SVG, SASS, TS, HTML → inline');
}

watchAll();
