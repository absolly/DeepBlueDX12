function main()
	relics = 0;
	message(100,"test2");
	message(1000, "test");

	visit("act1");
end

function act1()
	showObjectiveDistance(0);
end

function getCaveRelic()
	relics = relics + 1;
	visit("act2");
end

function getBoatRelic()
	relics = relics + 1;
	visit("act3");
end

function checkTempleDoor()
	if(relics > 1) then
		message("The temple door is opening..!!");
		openTempleDoor()
	end
end

function onTriggercollision()
	message("Welcome to the ship..!!");
end

function openTempleDoor()
end