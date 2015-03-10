var SPACE = um(20);
var X_COUNT = 10;
var Z_COUNT = 10;

for (var i = 0; i < X_COUNT; ++i)
{
	for (var j = 0; j < Z_COUNT; ++j)
	{
		new Yeast({
				x: -(SPACE * X_COUNT / 2) + SPACE * i,
				z: -(SPACE * Z_COUNT / 2) + SPACE * j,
				gfp: um(100) + um(10) * i * j,
				rfp: um(500) - um(10) * i * j,
				yfp: um(100) + Math.random() * um(200)
			},
			function () {	
				this.gfp += (Math.random() * 20 - 10);
				this.rfp += (Math.random() * 20 - 10);
				this.yfp += (Math.random() * 20 - 10);
				//this.volume += (Math.random() * um3(500));
			}
		);
	}
}