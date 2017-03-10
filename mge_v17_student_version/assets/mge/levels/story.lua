function main()
	reset();
end

function reset()
	RelicTabletPickedUp = false;
	RelicDiscPickedUp = false;
	RelicStatuePickedUp = false;
	RelicMirrorPickedUp = false;
	RelicKruikPickedUp = false;
	TempleKeyPickedUp = false;
end

--------------TREASURE--------------
function onTreasureCollision(self, ePressed)
	if (ePressed) then
		addCoin();
		--destroyGroup("door1", true);
		--destroyGroup("door2", true);
		playSound("treasure_grab", "treasure_grab", false, true, 100, 0.8, 1.2, "");
		destroy(self);
	else
		setInteractionText("Press E to pick up the treasure");
	end
end
--------------TREASURE--------------

--------------RELICS--------------
function onRelic_tabletCollision(self, ePressed)
	if not (RelicTabletPickedUp) then
		if (ePressed) then
			addItemToInventory(self);
			RelicTabletPickedUp = true;
			playSound("relic_grab", "relic_grab", false, true, 100, 0.8, 1.2, "");
			playSound("(2) But what is this", "Voice line", false, true, 100, 1, 1, "But what is this! I've never seen anything like it! \nIt doesn't look humanmade.");
			destroyGroup("door1", true);
			destroy(self);
		else
			setInteractionText("Press E to pick up the relic");
		end
	end
end

function onRelic_kruikCollision(self, ePressed)
	if not (RelicKruikPickedUp) then
		if (ePressed) then
			addItemToInventory(self);
			RelicKruikPickedUp = true;
			playSound("relic_grab", "relic_grab", false, true, 100, 0.8, 1.2, "");
			destroy(self);
		else
			setInteractionText("Press E to pick up the relic");
		end
	end
end

function onRelic_mirrorCollision(self, ePressed)
	if not (RelicMirrorPickedUp) then
		if (ePressed) then
			addItemToInventory(self);
			RelicMirrorPickedUp = true;
			playSound("relic_grab", "relic_grab", false, true, 100, 0.8, 1.2, "");
			destroy(self);
		else
			setInteractionText("Press E to pick up the relic");
		end
	end
end

function onTemple_keyCollision(self, ePressed)
	if not (TempleKeyPickedUp) then
		if (ePressed) then
			addItemToInventory(self);
			TempleKeyPickedUp = true;
			playSound("relic_grab", "relic_grab", false, true, 100, 0.8, 1.2, "");
			destroy(self);
		else
			setInteractionText("Press E to pick up the key");
		end
	end
end

function onRelic_discCollision(self, ePressed)
	if not (RelicDiscPickedUp) then
		if (ePressed) then
			addItemToInventory(self);
			RelicDiscPickedUp = true;
			playSound("relic_grab", "relic_grab", false, true, 100, 0.8, 1.2, "");
			if (RelicTabletPickedUp and RelicDiscPickedUp and RelicStatuePickedUp) then
				onThreeRelicsPickedUp();
			end
			destroy(self);
		else
			setInteractionText("Press E to pick up the relic");
		end
	end
end

function onRelic_statueCollision(self, ePressed)
	if not (RelicStatuePickedUp) then
		if (ePressed) then
			addItemToInventory(self);
			RelicStatuePickedUp = true;
			playSound("relic_grab", "relic_grab", false, true, 100, 0.8, 1.2, "");
			if (RelicTabletPickedUp and RelicDiscPickedUp and RelicStatuePickedUp) then
				onThreeRelicsPickedUp();
			end
			destroy(self);
		else
			setInteractionText("Press E to pick up the relic");
		end
	end
end
	
function onThreeRelicsPickedUp()
	playSound("(7) These artifacts combined", "Voice line", false, true, 100, 1, 1, "These artifacts combined apparently form a key.\nThere must be a structure nearby where we can use them.");
	destroyGroup("door2", true);
end

function onTempleKeyCollision(self, ePressed)
	if not (RelicTabletPickedUp) then
		if (ePressed) then
			addItemToInventory(self);
			playSound("relic_grab", "relic_grab", false, true, 100, 0.8, 1.2, "");
			playSound("(10) Nice that should do the trick", "Voice line", false, true, 100, 1, 1 , "Nice, that should do the trick!");
			destroy(self);
		else
			setInteractionText("Press E to pick up the relic");
		end
	end
end
--------------RELICS--------------

function onTrigger1Collision(self, ePressed)
	playSound("(1) Alright its a beautiful day", "Voice line", false, true, 100, 1, 1, "Alright, it's a beatiful day for some diving! Let's start by \nexploring the cave, we might be able to find some kind of treasure. \nI'll send you the coordinates, and don't worry. I'll be following you with the boat.");
	destroy(self);
end

function onTrigger2Collision(self, ePressed)
	--playSound("(2) But what is this", "Voice line", false, true, 100);
	--setSubtitleText("But what is this! I've never seen anything like it! \nIt doesn't look humanmade.");
	--destroyGroup("door1", true);
	--destroy(self);
end

function onTrigger3Collision(self, ePressed)
	if (RelicTabletPickedUp) then
		playSound("(3) Ive deciphered", "Voice line", false, true, 100, 1, 1, "I've deciphered the strange singal coming from the artifact. \nIt has something to do with fish. Try and find some fish and  \nsee what the artifact does.");
		setFishHintText("Press Q to call for fish to protect you");
		destroy(self);
	end
end

function onTrigger4Collision(self, ePressed)
	playSound("(5) Ive picked up", "Voice line", false, true, 100, 1, 1, "I've picked up two strange signals from a nearby location. \nIt's the same as the artifact from the cave. I suggest we check it out. ");
	destroy(self);
end

function onTrigger5Collision(self, ePressed)
	playSound("(6) It appears", "Voice line", false, true, 100, 1, 1 , "It appears the signals both originate from somewhere in that sunken pirate ship.");
	destroy(self);
end

function onTrigger6Collision(self, ePressed)
	playSound("(8) Wow look", "Voice line", false, true, 100, 1, 1, "Wow, look at that temple!");
	destroy(self);
end

function onTrigger7Collision(self, ePressed)
	if (ePressed) then
		if (TempleKeyPickedUp) then
			--destroyGroup("door3", true);
			moveDown("door3", 700, 350);
			removeItemFromInventory("Temple_key.png");
			playSound("door_opening", "door opening", false, false, 100, 1, 1, "");
			destroy(self);
		else
			playSound("(9) Thats", "Voice line", false, false, 100, 1, 1 ,"That's weird, none of these relics fit in the slot on the door, \nmaybe look around and see if you can find something that fits.");
		end
	else
		setInteractionText("Press E to open the door.");
	end
end

function onTrigger8Collision(self, ePressed)
	playSound("(11) Wow look at the", "Voice line", false, true, 100, 1, 1, "Wow, look at the interior! \nThere must be a very valuable treasure stored around here.");
	destroy(self);
end

function onTrigger9Collision(self, ePressed)
	if (RelicMirrorPickedUp and RelicKruikPickedUp) then
		playSound("(13) Alright thats the last", "Voice line", false, false, 100, 1, 1, "That's the last of them. Let's place them in the pedestal and see what happens.");
		if (ePressed) then
			moveDown("door4", 800, 400);
			spawnGroup("Relics", true);
			removeItemFromInventory("Relic_tablet.png");
			removeItemFromInventory("Relic_disc.png");
			removeItemFromInventory("Relic_kruik.png");
			removeItemFromInventory("Relic_mirror.png");
			removeItemFromInventory("Relic_statue.png");
			playSound("door_opening", "door opening", false, false, 100, 1, 1, "");
			destroy(self);
		else
			setInteractionText("Press E to place the relics in the pedestals.");
		end
	elseif (RelicMirrorPickedUp) then
		playSound("(12) Judging from these - 1 left", "Voice line", false, false, 100, 1, 1, "Judging from these 5 pedestals, we need to find one more relic! \nI'd suggest you search the temple.");
	elseif (RelicKruikPickedUp) then
		playSound("(12) Judging from these - 1 left", "Voice line", false, false, 100, 1, 1, "Judging from these 5 pedestals, we need to find one more relic! \nI'd suggest you search the temple.");
	else
		playSound("(12) Judging from these", "Voice line", false, false, 100, 1, 1, "Judging from these 5 pedestals, we need to find 2 more relics! \nI'd suggest you search the temple.");
	end
end

function onTrigger10Collision(self, ePressed)
	playSound("Be careful out there", "Voice line", false, true, 100, 1, 1 ,"Be careful out there. \nI've heard there's a weird creature swimming around.");
	destroy(self);
end

function onTrigger11Collision(self, ePressed)
	playSound("The current", "Voice line", false, true, 100, 1, 1, "The current is getting too strong, \nI suggest you head back.");
	destroy(self);
end

function onTrigger12Collision(self, ePressed)
	onCollisionWithTrigger12(self);
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