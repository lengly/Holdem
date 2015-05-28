default:
	g++ ../source/game.cpp ../source/player.cpp ../source/card.cpp -o ../target/game
	#g++ server.cpp -o server

mov:
	cp ../target/game ~/git/run_area/

replay:
	mv ~/git/run_area/server/replay.txt ~/git/ViewClientForReplay/
	firefox viewclient.html

test:
	g++ ../source/test.cpp ../source/card.cpp -o test
	./test
	rm test

clean:
	rm -f ../target/game