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

--------------RELICS--------------
function onTreasureCollision(self, ePressed)
	if (ePressed) then
		playSound("environment_whale", "Coin", false, true, 100);
		addCoin();
		destroy(self);
		setInteractionText("");
	else
		setInteractionText("Press E to pick up the treasure");
	end
end

--------------RELICS--------------
function onRelic_tabletCollision(self, ePressed)
	print("onRelic_tabletCollision");
end

function onRelic_discCollision(self, ePressed)
	print("onRelic_discCollision");
end

function onRelic_statueCollision(self, ePressed)
	print("onRelic_statueCollision");
end
--------------RELICS--------------

function onTrigger1Collision(self, ePressed)
	playSound("Alright its a beautiful day", "Voice line", false, true, 100);
	setSubtitleText("Alright, it's a beatiful day for some diving! Let's start by \n exploring the cave, we might be able to find some kind of treasure. \n I'll send you the coordinates, and don't worry. I'll be following you with the boat.");
	destroy(self);
end

function onTrigger2Collision(self, ePressed)
	playSound("But what is this", "Voice line", false, true, 100);
	setSubtitleText("But what is this! I've never seen anything like it! \n It doesn't look humanmade.");
	destroyGroup("door1");
	destroy(self);
end

function onTrigger3Collision(self, ePressed)
	playSound("Ive deciphered", "Voice line", false, true, 100);
	setSubtitleText("I've deciphered the strange singal coming from the artifact. \n It has something to do with fish. Try and find some fish and  \n see what the artifact does.");
	destroy(self);
end

function onAct1_1Collision(self, ePressed)
	--playSound("environment_sonar", "environment_sonar", false, true, 100);
	--destroyGroup("Door1");
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