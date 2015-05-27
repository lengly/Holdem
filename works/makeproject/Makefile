default:
	g++ ../source/game.cpp ../source/player.cpp ../source/card.cpp -o ../target/game
	#g++ server.cpp -o server

mov:
	mv ../source/game ~/git/run_area/

replay:
	mv ~/git/run_area/server/replay.txt ~/git/ViewClientForReplay/
	firefox viewclient.html

clean:
	rm -f ../target/game