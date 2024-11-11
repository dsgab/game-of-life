package main

import (
	"fmt"
	"math/rand"
	"runtime"
	"time"
)

// var verbose *bool = flag.Bool("v", false, "prints the number of iterations per second")

func main() {
	var boardSideLength int = 1 << 9
	var nIterations int = 1 << 10
	var nWorkers = runtime.GOMAXPROCS(0)
	var prob float64 = 0.5

	fmt.Printf("Board Size: %d, nIterations: %d, nWorkers: %d\n", boardSideLength, nIterations, nWorkers)

	b1 := CreateBoard(boardSideLength, nWorkers)
	b2 := CreateBoard(boardSideLength, nWorkers)

	//populate both boards simultaneously
	for i := 0; i < boardSideLength; i++ {
		for j := 0; j < boardSideLength; j++ {
			if rand.Float64() <= prob {
				b1.Boards[0][i*boardSideLength+j] = 1
				b2.Boards[0][i*boardSideLength+j] = 1
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

	//Sequential time!
	start = time.Now()
	for i := 0; i < nIterations; i++ {
		b2.IterateSequentially()
	}
	t = time.Now()
	fmt.Println("Sequential program elapsed: ", t.Sub(start).String())

}
