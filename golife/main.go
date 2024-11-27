package main

import (
	"fmt"
	"math/rand"
	"time"
)

/*
func board_different(b1 *Board, b2 *Board) bool {
	if b1.sideLength != b2.sideLength {
		return true
	}

	for i := 0; i < b1.sideLength*b1.sideLength; i++ {
		if b1.Boards[0][i] != b2.Boards[0][i] {
			return true
		}
	}
	return false
}
*/


func PrintBoard(b *Board) {
	for i := 0; i < b.sideLength; i++ {
		for j := 0; j < b.sideLength; j++ {
			fmt.Printf("%d", b.Boards[0][i*b.sideLength+j])
		}
		fmt.Println()
	}
}

func main() {
	var boardSideLength int = 1 << 10
	var nIterations int = 1 << 10
	var nWorkers = 8
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

	var start time.Time
	var t time.Time

	// Concurrent time
	start = time.Now()
	for i := 0; i < nIterations; i++ {
		b2.IterateConcurrentlyByLine(nWorkers)
	}
	t = time.Now()
	fmt.Println("Concurrent program elapsed: ", t.Sub(start).String())

	//Pool Concurrent time
	start = time.Now()
	for i := 0; i < nIterations; i++ {
		bp.Iterate()

	}
	bp.ReleaseWorkers()
	t = time.Now()
	fmt.Println("Pool Concurrent program elapsed: ", t.Sub(start).String())

	//Sequential time!
	start = time.Now()
	for i := 0; i < nIterations; i++ {
		b1.IterateSequentially()
	}
	t = time.Now()
	fmt.Println("Sequential program elapsed: ", t.Sub(start).String())
}
