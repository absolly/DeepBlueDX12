function main()
	relics = 0;
	breathing = true;
	playSound("ambient_underwater", "ambient", true, false, 100);
	visit("act1");
end

function act1()
	showObjectiveDistance(0);
	playBreath();
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
	playSound("environment_sonar", "environment", false, true, 100);
end

function openTempleDoor()
end