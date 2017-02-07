function setup()
	beaverAlive = true
	birdAlive = true
	hasSword = false
	visit("main")
end

function handleDialogue()
	
	-- TALKING TO BEAVER --
	if isTalkingTo(Beaver) then
	
		-- DIALOGUE 1 --
		if dialogue == 1 then
			local response = saySelf("Hi there!\rHow are you doing?\rI hope you're doing well!")
			if response == 1 then nextDialogue() end
			
		-- DIALOGUE 2 --
		elseif dialogue == 2 then
			local response = sayOther("[Choose an option]","I'm doing well, thanks for asking!", "I'm not doing too great..")
			if response == 1 then setDialogue(3) end
			if response == 2 then setDialogue(4) end
		
		-- DIALOGUE 3 --
		elseif dialogue == 3 then
			local response = saySelf("That's good to hear! I have to go\rnow though, bye!")
			if response == 1 then setDialogue(5) end
		
		-- DIALOGUE 4 --
		elseif dialogue == 4 then
			local response = sayOther("That sucks to hear.. I have to go\rnow though, bye!")
			if response == 1 then setDialogue(5) end
		
		-- DIALOGUE 5 --
		elseif dialogue == 5 then
			local response = sayOther("Okey bye!")
			if response == 1 then setDialogue(1) talkTo(Fox) end
		end
	
	
	-- TALKING TO FOX --
	elseif isTalkingTo(Fox) then
	
		-- DIALOGUE 1 --
		if dialogue == 1 then
			local response = sayOther("I'm a warrior.")
			if response == 1 then setDialogue(2) end
		
		-- DIALOGUE 2 --
		elseif dialogue == 2 then
			local response = sayOther("Here is a sword!")
			if response == 1 then hasSword = true setDialogue(3) end
		
		-- DIALOGUE 3 --
		elseif dialogue == 3 then
			local response = saySelf("[Choose an option]", "Wtf am I supposed to do with this?", "Oh. Thanks", "*Stab the beaver*")
			if response == 1 then setDialogue(4)
			elseif response == 2 then setDialogue(5)
			elseif response == 3 then setDialogue(6) end
	
		-- DIALOGUE 4 --
		elseif dialogue == 4 then
			local response = sayOther("I dunno, I can't use it either, so bye!")
			if response == 1 then endDialogue() end
		
		-- DIALOGUE 5 --
		elseif dialogue == 5 then
			local response = sayOther("Ungrateful much? ")
			if response == 1 then endDialogue() end
		
		-- DIALOGUE 6 --
		elseif dialogue == 6 then
			local response = sayOther("Wow. You're such an evil human being!") 
			if response == 1 then beaverAlive = false endDialogue() end
		end

	-- TALKING TO BIRD --
	elseif isTalkingTo(Bird) then
		if hasSword then
			-- DIALOGUE 1 --
			if dialogue == 1 then
				local response = saySelf("Stab him?", "Nah..", "HELL YEA")
				if response == 1 then endDialogue() end
				if response == 2 then birdAlive = false endDialogue() end
			end
		else
			-- DIALOGUE 1 --
			if dialogue == 1 then
				local response = sayOther("I'm a shop owner ffs")
				if response == 1 then endDialogue() end
			end
		end
	end
end


------------------------------------------------------------------------------------------------------------------------
--														main()
------------------------------------------------------------------------------------------------------------------------
function main()
	image("background.png", 0, 162)
	if beaverAlive and button("Beaver.png", 100, 100) then talkTo(Beaver) end
	if birdAlive and button("Bird.png", 400, 100) then talkTo(Bird) end
	time = 0
end

------------------------------------------------------------------------------------------------------------------------
--														hallway()
------------------------------------------------------------------------------------------------------------------------
function hallway()
	image("empty_room.png", 0, 0)
	if button("door_orange.png", 222, 228) then
		visit("dragonroom")
	end
	if button("door_green.png", 445, 226) then
		visit("closetroom")
	end
end

------------------------------------------------------------------------------------------------------------------------
--														dragonroom()
------------------------------------------------------------------------------------------------------------------------
function dragonroom()
	image("empty_room.png", 0, 0)
	if button("window.png", 389, 197) then
		if monsterIsDead == true then
			visit("win")
		end
	end
	time = time + 1
	if monsterIsDead == false then
		if button("dragon.png", 0, math.sin(time*0.1)*25) then
			if hasSword then
				monsterIsDead = true
			else
				visit("dead")
			end
		end
	else
		image("dragon_dead.png", 0, 0)
	end
end

------------------------------------------------------------------------------------------------------------------------
--														closetroom()
------------------------------------------------------------------------------------------------------------------------
function closetroom()
	image("empty_room.png", 0, 0)
	if button("closet.png", 168, 120) then
		visit("closetopen")
	end
	if button("door_grey.png", 445, 226) then
		visit("hallway")
	end
end

------------------------------------------------------------------------------------------------------------------------
--														closetopen()
------------------------------------------------------------------------------------------------------------------------
function closetopen()
	image("closet_open.png", 0, 0)
	if hasSword == false then
		if button("sword.png", 219, 325) then
			hasSword = true
		end
	end
	if button("poison.png", 499, 82) then
		visit("dead")
	end
	if button("closebutton.png", 0, 0) then
		visit("closetroom")
	end
end

------------------------------------------------------------------------------------------------------------------------
--														dead()
------------------------------------------------------------------------------------------------------------------------
function dead()
	if button("endlose.png", 0, 0) then
		visit("main")
	end
end

------------------------------------------------------------------------------------------------------------------------
--														win()
------------------------------------------------------------------------------------------------------------------------
function win()
	if button("endwin.png", 0, 0) then
		visit("main")
	end
end
