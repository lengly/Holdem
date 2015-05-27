default:
	g++ game.cpp player.cpp card.cpp -o game
	g++ server.cpp -o server

mov:
	mv game ~/git/run_area/

replay:
	mv ~/git/run_area/server/replay.txt ~/git/ViewClientForReplay/
	firefox viewclient.html

clean:
	rm -f game server