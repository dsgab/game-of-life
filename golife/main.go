package main

import (
	"flag"
	"fmt"
	"log"
	"math/rand"
	"os"
	"strconv"
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

var verboseFlag *bool = flag.Bool("v", false, "if set prints neatly the time of each execution, e.g ./golife -v 100 100 10")

func main() {
	flag.Parse()
	args := flag.Args()
	if len(args) < 3 {
		log.Fatalf("Missing arguments: %s <BOARD_LENGTH> <NUMBER_ITERATIONS> <NUMBER_THREADS>\n", os.Args[0])
	}

	boardSideLength, err := strconv.Atoi(args[0])
	if err != nil {
		log.Fatalf("Error reading <BOARD_LENGTH>, read: %s\n", args[0])
	}

	nIterations, err := strconv.Atoi(args[1])
	if err != nil {
		log.Fatalf("Error reading <NUMBER_ITERATIONS>, read: %s\n", args[1])
	}

	nWorkers, err := strconv.Atoi(args[2])
	if err != nil {
		log.Fatalf("Error reading <NUMBER_THREADS>, read: %s\n", args[2])
	}

	var prob float64 = 0.5

	if *verboseFlag {
		fmt.Printf("Board Size: %d, nIterations: %d, nWorkers: %d\n", boardSideLength, nIterations, nWorkers)
	}

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

	durations := make([]time.Duration, 0, 3)

	var start time.Time
	var t time.Time

	// Concurrent time
	start = time.Now()
	for i := 0; i < nIterations; i++ {
		b2.IterateConcurrentlyByLine(nWorkers)
	}
	t = time.Now()

	durations = append(durations, t.Sub(start))

	if *verboseFlag {
		fmt.Println("Concurrent program elapsed: ", durations[0])
	}

	//Pool Concurrent time
	start = time.Now()
	for i := 0; i < nIterations; i++ {
		bp.Iterate()

	}
	bp.ReleaseWorkers()
	t = time.Now()

	durations = append(durations, t.Sub(start))

	if *verboseFlag {
		fmt.Println("Pool Concurrent program elapsed: ", durations[1])
	}

	//Sequential time!
	start = time.Now()
	for i := 0; i < nIterations; i++ {
		b1.IterateSequentially()
	}
	t = time.Now()

	durations = append(durations, t.Sub(start))

	if *verboseFlag {
		fmt.Println("Sequential program elapsed: ", durations[2])
	}

	if !(*verboseFlag) {
		fmt.Println(durations[0].Seconds())
		fmt.Println(durations[1].Seconds())
		fmt.Println(durations[2].Seconds())
	}

}
