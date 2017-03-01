function main()
	relics = 0;
	
	message("test");
	visit("act1");
end

function act1()
	showObjectiveDistance(0);
	message("testing the messageBox");
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

function openTempleDoor()
end