import base64

html_path = 'ColorAlchemy.html'
js_path = 'ColorAlchemy.js'
wasm_path = 'ColorAlchemy.wasm'
qtloader_path = 'qtloader.js'
qtlogo_path = 'qtlogo.svg'
output_path = 'ColorAlchemy_standalone.html'

# Read original HTML
with open(html_path, 'r') as f:
    html_content = f.read()

# Inline WASM as data URI
with open(wasm_path, 'rb') as f:
    wasm_data = f.read()
    wasm_data_uri = 'data:application/wasm;base64,' + base64.b64encode(wasm_data).decode('utf-8')

# Inline main JS, switching WASM URL to data URI
with open(js_path, 'r') as f:
    js_content = f.read()
    js_content = js_content.replace('ColorAlchemy.wasm', wasm_data_uri)

# Prepare qtloader, patching XHR for data URLs
with open(qtloader_path, 'r') as f:
    qtloader_content = f.read()
    qtloader_content = qtloader_content.replace(
        'var xhr = new XMLHttpRequest();',
        'if (url.startsWith("data:")) { return url; } var xhr = new XMLHttpRequest();'
    )

# Inline SVG logo
with open(qtlogo_path, 'rb') as f:
    logo_data = f.read()
    logo_data_uri = 'data:image/svg+xml;base64,' + base64.b64encode(logo_data).decode('utf-8')

# Replace script tags in HTML with inline code
html_content = html_content.replace(
    '<script src="qtloader.js"></script>',
    f'<script>\n// Inlined qtloader.js\n{qtloader_content}\n</script>'
)
html_content = html_content.replace(
    '<script src="ColorAlchemy.js"></script>',
    f'<script>\n// Inlined ColorAlchemy.js\n{js_content}\n</script>'
)
html_content = html_content.replace('qtlogo.svg', logo_data_uri)

# Escape backticks in qtloader for f-string
safe_qtloader = qtloader_content.replace('`', '\\`')

# Worker override snippet (inject before fetch override)
worker_override = f'''
<script>
// Blob-based override for qtloader.js Worker
const qtLoaderCode = `{safe_qtloader}`;
const qtLoaderBlob = new Blob([qtLoaderCode], {{ type: 'application/javascript' }});
const qtLoaderURL  = URL.createObjectURL(qtLoaderBlob);
const NativeWorker = window.Worker;
window.Worker = function(scriptURL, options) {{
    if (typeof scriptURL === 'string' && scriptURL.endsWith('qtloader.js')) {{
        return new NativeWorker(qtLoaderURL, options);
    }}
    return new NativeWorker(scriptURL, options);
}};
</script>
'''

# Existing fetch and URL override for WASM data URI
override_script = '''
<script>
const OriginalURL = URL;
URL = function(url, base) {
    if (url && url.toString().startsWith('data:application/wasm')) {
        return {
            toString: function() { return url; },
            href: url
        };
    }
    return new OriginalURL(url, base);
};

const originalFetch = window.fetch;
window.fetch = function(url, options) {
    if (url && url.toString().startsWith('data:application/wasm')) {
        const base64Data = url.split(',')[1];
        const binaryString = atob(base64Data);
        const bytes = new Uint8Array(binaryString.length);
        for (let i = 0; i < binaryString.length; i++) {
            bytes[i] = binaryString.charCodeAt(i);
        }
        return Promise.resolve(new Response(bytes.buffer, {
            status: 200,
            headers: new Headers({
                'Content-Type': 'application/wasm'
            })
        }));
    }
    return originalFetch(url, options);
};
</script>
'''

# Inject both overrides into the HTML body
html_content = html_content.replace('<body>', '<body>\n' + worker_override + override_script)

# Write out the standalone HTML
with open(output_path, 'w') as f:
    f.write(html_content)

print(f"Created standalone file: {output_path}")