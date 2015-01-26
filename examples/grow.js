var GROW_RATE = 5000;

for (var i = 0; i < 5; ++i)
{
	new Cell({
			rfp: 1000,
			yfp: 1000,
			gfp: 1000
		}, 
		function () {
			this.volume += Math.random() * GROW_RATE;
		}
	);
}