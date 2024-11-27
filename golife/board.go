package main

import (
	"math"
	"sync"
)

type Board struct {
	// gophers    int
	sideLength int
	Boards     [2][]int
}

func isPower2(n int) bool {
	if n < 1 {
		return false
	}

	for n > 1 {
		if n%2 == 1 {
			return false
		}
		n /= 2
	}
	return true
}

func CreateBoard(sideLength int) Board {
	//check if gophers and sideLength are powers of two
	/*
		if !isPower2(gophers) {
			panic("ERROR: Number of workers MUST be a power of 2!")
		}

		if gophers > sideLength*sideLength {
			panic("ERROR: More Goroutines than BOARD CELLS")
		}
	*/
	boards := [...][]int{make([]int, sideLength*sideLength), make([]int, sideLength*sideLength)}
	return Board{
		// gophers:    gophers,
		sideLength: sideLength,
		Boards:     boards,
	}
}

// linha i coluna j
func (b *Board) numNeighbors(i, j int) int {
	n := 0
	size := b.sideLength
	for y := -1; y <= 1; y++ {
		yi := (i + y + b.sideLength) % b.sideLength
		for x := -1; x <= 1; x++ {
			if x == 0 && y == 0 {
				continue
			}

			xj := (x + j + b.sideLength) % b.sideLength

			n += b.Boards[0][yi*size+xj]
		}
	}

	return n
}

// returns number of alive cells in table!
// linha i coluna j
func (b *Board) subTableWork(i, j, height, width int) int {
	alive := 0
	side := b.sideLength

	for y := i; y < i+height; y++ {
		for x := j; x < j+width; x++ {
			nn := b.numNeighbors(y, x)

			if b.Boards[0][y*side+x] == 1 {
				if nn < 2 || nn > 3 {
					//se vivo, mas menos de 2 ou mais de 3 vizinhos, morre
					b.Boards[1][y*side+x] = 0
				} else {
					b.Boards[1][y*side+x] = 1
					alive++
				}

			} else {
				if nn == 3 {
					//se morto, mas com 3 vizinhos, vive
					b.Boards[1][y*side+x] = 1
					alive++
				} else {
					b.Boards[1][y*side+x] = 0
				}
			}
		}
	}
	return alive
}

func (b *Board) swapBoards() {
	b.Boards[0], b.Boards[1] = b.Boards[1], b.Boards[0]
}

// inicialmente, maneira mais boba possivel, divide o tabuleiro entre os gophers
func (b *Board) IterateSequentially() {
	b.subTableWork(0, 0, b.sideLength, b.sideLength)
	b.swapBoards()
}

// Argument MUST be a power of 2!
func (b *Board) determineGopherTableSize(gophers int) (int, int) {
	if !isPower2(b.sideLength) || !isPower2(gophers) {
		return 1, b.sideLength
	}

	sqrRoot := int(math.Sqrt(float64(gophers)))
	width, height := b.sideLength, b.sideLength
	if sqrRoot*sqrRoot != int(gophers) { // i.e gophers is of the form 2 ^ (2k + 1)
		height /= 2
		sqrRoot = int(math.Sqrt(float64(gophers / 2)))
	}

	width /= sqrRoot
	height /= sqrRoot

	if width == 0 {
		width = 1
	}
	if height == 0 {
		height = 1
	}

	return height, width
}

// The number of goroutines must be a power of 2
func (b *Board) IterateConcurrently(gophers int) {
	// test if number of gophers is square!
	width, height := b.determineGopherTableSize(gophers)

	var wg sync.WaitGroup
	for y := 0; y < b.sideLength; y += height {
		for x := 0; x < b.sideLength; x += width {
			wg.Add(1)

			//asks for a goroutine to do a table!
			go func(i, j int) {
				b.subTableWork(i, j, height, width)
				wg.Done()
			}(y, x)

		}
	}

	//wait for all goroutines!
	wg.Wait()
	b.swapBoards()
}

func (b *Board) IterateConcurrentlyByLine(gophers int) {
	// test if number of gophers is square!
	var wg sync.WaitGroup
	for y := 0; y < b.sideLength; y++ {
		wg.Add(1)
		go func(i, j int) {
			b.subTableWork(i, j, 1, b.sideLength)
			wg.Done()
		}(y, 0)
	}

	//wait for all goroutines!
	wg.Wait()
	b.swapBoards()
}
