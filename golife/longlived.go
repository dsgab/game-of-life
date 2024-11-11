package main

import "sync"

// Esse arquivo implementa outra forma de concorrência
// Onde não chamaremos as goroutines pelas chamadas go
// E sim, somente por comunicação. O número de goroutines
// se manterá constante durante a execução.

type workArea struct {
	x, y          int
	width, height int
}

type BoardPool struct {
	board       Board
	numWorkers  int
	taskManager chan workArea
	wg          sync.WaitGroup
}

func (bp *BoardPool) gopherRoutine() {
	for wa := range bp.taskManager {
		bp.board.subTableWork(wa.x, wa.y, wa.width, wa.height)
		bp.wg.Done()
	}
}

// numWorkers must be a power of 2
func CreateBoardPool(sideLength int, numWorkers int) *BoardPool {
	if !isPower2(numWorkers) {
		panic("numWorkers MUST be a power of 2!")
	}

	bp := BoardPool{
		board:       CreateBoard(sideLength, numWorkers),
		numWorkers:  numWorkers,
		taskManager: make(chan workArea, numWorkers),
		wg:          sync.WaitGroup{},
	}

	for i := 0; i < numWorkers; i++ {
		go bp.gopherRoutine()
	}

	return &bp
}

func (bp *BoardPool) ReleaseWorkers() {
	close(bp.taskManager)
}
