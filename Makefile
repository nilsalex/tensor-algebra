CC=g++
CFLAGS=-std=c++14 -ggdb

all: main

main: main.o Rational.o Indices.o Tensor.o TensorMonomial.o Scalar.o ScalarSum.o MonomialExpression.o Expression.o
	$(CC) -o main main.o Rational.o Indices.o Tensor.o TensorMonomial.o Scalar.o ScalarSum.o MonomialExpression.o Expression.o $(CFLAGS)

Rational.o: Rational.cpp Rational.h
	$(CC) -c -o $@ $< $(CFLAGS)

Indices.o: Indices.cpp Indices.h
	$(CC) -c -o $@ $< $(CFLAGS)

Tensor.o: Tensor.cpp Tensor.h Indices.h
	$(CC) -c -o $@ $< $(CFLAGS)

TensorMonomial.o: TensorMonomial.cpp TensorMonomial.h Tensor.h Indices.h
	$(CC) -c -o $@ $< $(CFLAGS)

Scalar.o: Scalar.cpp Scalar.h Rational.h
	$(CC) -c -o $@ $< $(CFLAGS)

ScalarSum.o: ScalarSum.cpp ScalarSum.h Scalar.h
	$(CC) -c -o $@ $< $(CFLAGS)

MonomialExpression.o: MonomialExpression.cpp MonomialExpression.h Tensor.h Indices.h
	$(CC) -c -o $@ $<  $(CFLAGS)

Expression.o: Expression.cpp Expression.h MonomialExpression.h ScalarSum.h Scalar.h Indices.h
	$(CC) -c -o $@ $<  $(CFLAGS)

main.o: main.cpp Rational.h Indices.h Tensor.h TensorMonomial.h Scalar.h ScalarSum.h MonomialExpression.h Expression.h
	$(CC) -c -o $@ $<  $(CFLAGS)

clean:
	rm -f main.o Indices.o Tensor.o TensorMonomial.o Scalar.o ScalarSum.o MonomialExpression.o Expression.o Rational.o main
