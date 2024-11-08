package main

import (
	"math"
	"sync"
)

type Board struct {
	gophers    int
	sideLength int
	Boards     [2][]int
	// doneChannel chan struct{}
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

func CreateBoard(sideLength int, gophers int) Board {
	//check if gophers and sideLength are powers of two
	if !isPower2(sideLength) {
		panic("ERROR: Board Side Length MUST be a power of 2!")
	}

	if !isPower2(gophers) {
		panic("ERROR: Number of workers MUST be a power of 2!")
	}

	if gophers > sideLength*sideLength {
		panic("ERROR: More Goroutines than BOARD CELLS")
	}

	boards := [...][]int{make([]int, sideLength*sideLength), make([]int, sideLength*sideLength)}
	return Board{
		gophers:    gophers,
		sideLength: sideLength,
		Boards:     boards,
		// doneChannel: make(chan struct{}),
	}
}

func (b *Board) numNeighbohrs(i, j int) int {
	n := 0
	size := b.sideLength
	for x := -1; x <= 1; x++ {
		xi := (i + x + b.sideLength) % b.sideLength
		for y := -1; y <= 1; y++ {
			if x == 0 && y == 0 {
				continue
			}
			yj := (j + y + b.sideLength) % b.sideLength

			n += b.Boards[0][yj*size+xi]
		}
	}

	return n
}

// inicialmente, maneira mais boba possivel, divide o tabuleiro entre os gophers
func (b *Board) IterateSequentially() {
	size := b.sideLength
	for i := 0; i < b.sideLength; i++ {
		for j := 0; j < b.sideLength; j++ {
			nn := b.numNeighbohrs(i, j)

			if b.Boards[0][i*size+j] == 1 {
				if nn < 2 || nn > 3 {
					//se vivo, mas menos de 2 ou mais de 3 vizinhos, morre
					b.Boards[1][i*size+j] = 0
				}
			} else if nn == 3 {
				//se morto, mas com 3 vizinhos, vive
				b.Boards[1][i*size+j] = 1
			}

		}
	}
	b.Boards[0], b.Boards[1] = b.Boards[1], b.Boards[0]
}

func (b *Board) subTableWork(x, y, dwidth, dheight int) {
	size := b.sideLength

	for i := y; i < dheight; i++ {
		for j := x; j < dwidth; j++ {
			nn := b.numNeighbohrs(i, j)

			if b.Boards[0][i*size+j] == 1 {
				if nn < 2 || nn > 3 {
					//se vivo, mas menos de 2 ou mais de 3 vizinhos, morre
					b.Boards[1][i*size+j] = 0
				}
			} else if nn == 3 {
				//se morto, mas com 3 vizinhos, vive
				b.Boards[1][i*size+j] = 1
			}
		}
	}

}

func (b *Board) determineGopherTableSize() (int, int) {
	sqrRoot := int(math.Sqrt(float64(b.gophers)))
	width, height := b.sideLength, b.sideLength
	if sqrRoot*sqrRoot != int(b.gophers) { // i.e gophers is of the form 2 ^ (2k + 1)
		height /= 2
		sqrRoot = int(math.Sqrt(float64(b.gophers / 2)))
	}

	width /= sqrRoot
	height /= sqrRoot

	return width, height
}

func (b *Board) IterateConcurrently() {
	// test if number of gophers is square!
	width, height := b.determineGopherTableSize()

	var wg sync.WaitGroup
	for x := 0; x < b.sideLength; x += width {
		for y := 0; y < b.sideLength; y += height {
			wg.Add(1)

			//asks for a goroutine to do a table!
			go func(i, j int) {
				b.subTableWork(i, j, width, height)
				wg.Done()
			}(x, y)

		}
	}

	//wait for all goroutines!
	wg.Wait()
	b.Boards[0], b.Boards[1] = b.Boards[1], b.Boards[0]

}
