var GROW_RATE = ul(6000);


new Yeast({
		rfp: 5000,
		yfp: 1000,
		gfp: 1000
	}, 
	function () {
		//console.log("[", this.id, "]: Start ", stepNumber);

		if (this.bud)
		{
			//console.log("[", this.id, "]: Has bud");

			this.bud.volume += ul(Math.random() * GROW_RATE);

			if (this.bud.volume >= ul(30000))
			{
				console.log("[", this.id, "]: Bud release");
				this.budRelease();
			}
		}
		else if (this.volume <= ul(80000))
		{
			this.volume += Math.random() * GROW_RATE;
			this.gfp += Math.random() * 2500;
			//console.log("[", this.id, "]: Has no bud");

			if (this.volume >= ul(80000)/* && Math.random() < 0.01*/)
			{
				console.log("[", this.id, "]: Bud create");
				this.budCreate();
			}
		}

		//console.log("[", this.id, "]: Stop ", stepNumber);
		//canvas.saveToFile("image" + stepNumber + ".jpg");
	}
);
