const blob = new Blob(
    [document.getElementById('box2d-worker')?.textContent!],
    { type: 'application/javascript' }
  );
  const worker = new Worker(URL.createObjectURL(blob));
  worker.onmessage = (e) => void console.log(e.data);
  worker.postMessage("weener");