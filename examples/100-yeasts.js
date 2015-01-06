for (var i = 0; i < 10; ++i)
	for (var j = 0; j < 10; ++j)
		new yeast({
			x: 10 * i,
			y: 10 * j,
			gfp: 100 + 10 * i * j,
			rfp: 500 - 10 * i * j,
			yfp: 100 + rand() % 200
		});