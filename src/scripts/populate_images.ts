document.querySelectorAll<HTMLLinkElement>('link[as="image"]').forEach(async link => {
    const res = await fetch(link.getAttribute('href')!);
    const blob = await res.blob();
    const url = URL.createObjectURL(blob);
    document.documentElement.style.setProperty(link.id, `url(${url})`);
});