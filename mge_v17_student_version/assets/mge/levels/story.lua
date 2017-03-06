function main()
	
	visit("act1_1");
	message("Welcome to our playtest, please proceed to the objective and have fun!", 500);
end

function act1_1()
	showObjectiveDistance(0);
end

function act1_2()
	showObjectiveDistance(1);
end

function act2_1()
	showObjectiveDistance(2);
end

function onTreasureCollision(self, ePressed)
	if (ePressed) then
		playSound("ability.wav", "Coin", false, true, 100);
		addCoin();
		destroy(self);
		setInteractionText("");
	else
		setInteractionText("Press E to pick up the treasure");
	end
end

function onAct1_1Collision(self, ePressed)
	playSound("ability.wav", "Coin", false, true, 100);
	message("That cave might just have something interesting for me." + ePressed);
	destroyGroup("Door1");
	visit("act1_2");
	--destroy(self);
end

function onAct1_2Collision(self, ePressed)
	message("This artifact.. it doesn't look human made..?!");
	visit("act2_1");
end

function onAct1_3Collision(self, ePressed)
	message("I hole in the lower deck of the ship? Intersting..");
end