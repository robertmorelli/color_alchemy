import base64

html_path = 'ColorAlchemy.html'
js_path = 'ColorAlchemy.js'
wasm_path = 'ColorAlchemy.wasm'
qtloader_path = 'qtloader.js'
qtlogo_path = 'qtlogo.svg'
output_path = 'index.html'

with open(html_path, 'r') as f:
    html_content = f.read()

with open(wasm_path, 'rb') as f:
    wasm_data = f.read()
    wasm_data_uri = 'data:application/wasm;base64,' + base64.b64encode(wasm_data).decode('utf-8')

with open(js_path, 'r') as f:
    js_content = f.read()
    js_content = js_content.replace('ColorAlchemy.wasm', wasm_data_uri)

with open(qtloader_path, 'r') as f:
    qtloader_content = f.read()
    qtloader_content = qtloader_content.replace(
        'var xhr = new XMLHttpRequest();',
        'if (url.startsWith("data:")) { return url; } var xhr = new XMLHttpRequest();'
    )

with open(qtlogo_path, 'rb') as f:
    logo_data = f.read()
    logo_data_uri = 'data:image/svg+xml;base64,' + base64.b64encode(logo_data).decode('utf-8')

html_content = html_content.replace(
    '<script src="qtloader.js"></script>', 
    f'<script>\n// Inlined qtloader.js\n{qtloader_content}\n</script>'
)

html_content = html_content.replace(
    '<script src="ColorAlchemy.js"></script>', 
    f'<script>\n// Inlined ColorAlchemy.js\n{js_content}\n</script>'
)

html_content = html_content.replace('qtlogo.svg', logo_data_uri)

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

html_content = html_content.replace('<body>', '<body>\n' + override_script)

with open(output_path, 'w') as f:
    f.write(html_content)

print(f"Created standalone file: {output_path}")
print("This file contains everything needed to run your app.")
print("Just double-click it to open in a browser - NO SERVER REQUIRED.")
