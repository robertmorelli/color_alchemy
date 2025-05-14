
const wasm_funcs: Record<string, any> = {};
{
    const image_query = 'link[as="image"]';

    document.head.querySelectorAll<HTMLLinkElement>(image_query)
        .forEach(async ({ href, id }) => {
            const data = await fetch(href);
            const blob = await data.blob();
            const url = URL.createObjectURL(blob);
            document.documentElement.style.setProperty(id, `url(${url}`)
        });
}