document.addEventListener('DOMContentLoaded', async () => {
    Box2D.ready.then((v) => {
        var world = new v.b2World(new v.b2Vec2(0.0, -10.0));
        console.log(world);
    });
});