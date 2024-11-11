package main

import (
	"fmt"
	"math/rand"
	"runtime"
	"time"
)

// var verbose *bool = flag.Bool("v", false, "prints the number of iterations per second")

func main() {
	var boardSideLength int = 1 << 10
	var nIterations int = 1 << 10
	var nWorkers = runtime.GOMAXPROCS(0)
	var prob float64 = 0.5

	fmt.Printf("Board Size: %d, nIterations: %d, nWorkers: %d\n", boardSideLength, nIterations, nWorkers)

	b1 := CreateBoard(boardSideLength)
	b2 := CreateBoard(boardSideLength)
	bp := CreateBoardPool(boardSideLength, nWorkers)

	//populate all boards simultaneously
	for i := 0; i < boardSideLength; i++ {
		for j := 0; j < boardSideLength; j++ {
			if rand.Float64() <= prob {
				b1.Boards[0][i*boardSideLength+j] = 1
				b2.Boards[0][i*boardSideLength+j] = 1
				bp.board.Boards[0][i*boardSideLength+j] = 1
			}
		}
	}

	//Concurrent time
	start := time.Now()
	for i := 0; i < nIterations; i++ {
		b1.IterateConcurrently(nWorkers)
	}
	t := time.Now()
	fmt.Println("Concurrent program elapsed: ", t.Sub(start).String())

	//Concurrent pool
	start = time.Now()
	for i := 0; i < nIterations; i++ {
		bp.Iterate()
	}
	t = time.Now()
	fmt.Println("Pool Concurrent program elapsed: ", t.Sub(start).String())
	bp.ReleaseWorkers()

	//Sequential time!
	start = time.Now()
	for i := 0; i < nIterations; i++ {
		b2.IterateSequentially()
	}
	t = time.Now()
	fmt.Println("Sequential program elapsed: ", t.Sub(start).String())

}
