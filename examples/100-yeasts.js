var SPACE = 25;
var X_COUNT = 10;
var Z_COUNT = 10;

for (var i = 0; i < X_COUNT; ++i)
{
	for (var j = 0; j < Z_COUNT; ++j)
	{
		new Cell({
				x: -(SPACE * X_COUNT / 2) + SPACE * i,
				z: -(SPACE * Z_COUNT / 2) + SPACE * j,
				gfp: 100 + 10 * i * j,
				rfp: 500 - 10 * i * j,
				yfp: 100 + Math.random() * 200
			},
			function () {	
				this.gfp += (Math.random() * 20 - 10);
				this.rfp += (Math.random() * 20 - 10);
				this.yfp += (Math.random() * 20 - 10);
				this.volume += (Math.random() * 1000);
			}
		);
	}
}