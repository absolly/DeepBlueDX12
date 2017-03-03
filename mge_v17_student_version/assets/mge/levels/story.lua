function main()
	relics = 0;
	breathing = true;
	playSound("ambient_underwater", "ambient", true, false, 100);
	visit("act1");
	message("testing this msg",1000);
	message("testing this msg2",400);
end

function act1()
	showObjectiveDistance(0);
	playBreath();
	message("hi");
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