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

function onAct1_1Collision()
	message("That cave might just have something interesting for me.");
	destroyGroup("Door1");
	visit("act1_2");
end

function onAct1_2Collision()
	message("This artifact.. it doesn't look human made..?!");
	visit("act2_1");
end

function onAct1_3Collision()
	message("I hole in the lower deck of the ship? Intersting..");
end