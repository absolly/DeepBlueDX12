function main()
	RelicTabletPickedUp = false;
	RelicDiscPickedUp = false;
	RelicStatuePickedUp = false;
--	message("Welcome to our playtest, please proceed to the objective and have fun!", 500);
end

--function act1_1()
--	showObjectiveDistance(0);
--end

--function act1_2()
--	showObjectiveDistance(1);
--end

--function act2_1()
--	showObjectiveDistance(2);
--end

--------------TREASURE--------------
function onTreasureCollision(self, ePressed)
	if (ePressed) then
		playSound("environment_whale", "Coin", false, true, 100, "");
		addCoin();
		destroy(self);
		setInteractionText("");
	else
		setInteractionText("Press E to pick up the treasure");
	end
end
--------------TREASURE--------------

--------------RELICS--------------
function onRelic_tabletCollision(self, ePressed)
	RelicTabletPickedUp = true;
	playSound("(2) But what is this", "Voice line", false, true, 100, "But what is this! I've never seen anything like it! \n It doesn't look humanmade.");
	destroyGroup("door1");
	destroy(self);
end

function onRelic_discCollision(self, ePressed)
	RelicDiscPickedUp = true;
	print("onRelic_discCollision");
	destroy(self);
end

function onRelic_statueCollision(self, ePressed)
	RelicStatuePickedUp = true;
	print("onRelic_statueCollision");
	destroy(self);
end
--------------RELICS--------------

function onTrigger1Collision(self, ePressed)
	playSound("(1) Alright its a beautiful day", "Voice line", false, true, 100, "Alright, it's a beatiful day for some diving! Let's start by \n exploring the cave, we might be able to find some kind of treasure. \n I'll send you the coordinates, and don't worry. I'll be following you with the boat.");
	destroy(self);
end

function onTrigger2Collision(self, ePressed)
	--playSound("(2) But what is this", "Voice line", false, true, 100);
	--setSubtitleText("But what is this! I've never seen anything like it! \n It doesn't look humanmade.");
	--destroyGroup("door1");
	--destroy(self);
end

function onTrigger3Collision(self, ePressed)
	if (RelicTabletPickedUp) then
		playSound("(3) Ive deciphered", "Voice line", false, true, 100, "I've deciphered the strange singal coming from the artifact. \n It has something to do with fish. Try and find some fish and  \n see what the artifact does.");
		destroy(self);
	end
end

function onTrigger4Collision(self, ePressed)
	playSound("(5) Ive picked up", "Voice line", false, true, 100, "I've picked up two strange signals from a nearby location. \n It's the same as the artifact from the cave. I suggest we check it out. ");
	destroy(self);
end

function onTrigger5Collision(self, ePressed)
	playSound("(6) It appears", "Voice line", false, true, 100 , "It appears the signals both originate from somewhere in that sunken pirate ship.");
	destroy(self);
end

function onTrigger6Collision(self, ePressed)
	playSound("(8) Wow look", "Voice line", false, true, 100, "Wow, look at that temple!");
	destroy(self);
end

function onTrigger7Collision(self, ePressed)
	playSound("(9) Thats", "Voice line", false, true, 100 ,"That's weird, none of these relics fit in the slot on the door, \n maybe look around and see if you can find something that fits.");
	destroy(self);
end

function onTrigger8Collision(self, ePressed)
	playSound("(11) Wow look at the", "Voice line", false, true, 100, "Wow, look at the interior! \n There must be a very valuable treasure stored around here.");
	destroy(self);
end

function onTrigger9Collision(self, ePressed)
	playSound("(12) Judging from these", "Voice line", false, true, 100, "Judging from these 5 pedestals, we need to find 2 more relics! \n I'd suggest you search the temple.");
	destroy(self);
end

function onTrigger10Collision(self, ePressed)
	playSound("Be careful out there", "Voice line", false, true, 100 ,"Be careful out there. \n I've heard there's a weird creature swimming around.");
	destroy(self);
end

function onTrigger11Collision(self, ePressed)
	playSound("The current", "Voice line", false, true, 100, "The current is getting too strong, \n I suggest you head back.");
	destroy(self);
end