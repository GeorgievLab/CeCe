hela = new Cell({
	volume: 33510,
	rfp: 2000
});

for (var i = 0; i < 500; ++i)
{
	new Cell({
		x: 100 - Math.random() * 200,
		z: 100 - Math.random() * 200,
		volume: 82,
		gfp: 2000
	});
}
