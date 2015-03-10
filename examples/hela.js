hela = new Cell({
	y: 50,
	volume: 33510,
	rfp: 2000
});

for (var i = 0; i < 500; ++i)
{
	new Yeast({
		x: 100 - Math.random() * 200,
		y: 100 - Math.random() * 100,
		z: 100 - Math.random() * 200,
		volume: 82,
		gfp: 2000
	});
}
