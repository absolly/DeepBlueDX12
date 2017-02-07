------------------------------------------------------------------------------------------------------------------------
--														start()
------------------------------------------------------------------------------------------------------------------------
function start()
	print ("Welcome to this adventure. Find a coin!")

	player = createObject("player", 0, 0, 0, "red")
	coin = createObject("coin", 0, 0, 3, "yellow")
	key = createObject("key", 3, 0, 0, "grey")

	setmaincharacter(player);

	addlistener(player, key, "onPlayerKeyCollide")
	addlistener(player, coin, "onPlayerCoinCollide")

	hasKey = false
end

------------------------------------------------------------------------------------------------------------------------
--														onPlayerCoinCollide()
------------------------------------------------------------------------------------------------------------------------
function onPlayerKeyCollide(player, key)
	print("you found a key! Good job!")
	destroy(key)
	hasKey = true
end

------------------------------------------------------------------------------------------------------------------------
--														onPlayerCoinCollide()
------------------------------------------------------------------------------------------------------------------------
function onPlayerCoinCollide(player, coin)
	if hasKey == true then
		print("you found the coin! Game complete!")
		destroy(coin);
	else
		print("nice try, but you need to find the key first..")
	end
end
