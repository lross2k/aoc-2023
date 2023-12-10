cflags := -Werror -Wall -Wshadow -pedantic -std=c89
cache_dir := .cache
src_dir := .
out_file := app.out
include_dir := ../include

build: camel_cards
	cc ${cache_dir}/camel_cards.o ${cflags} -I${include_dir} -o ${out_file} 

camel_cards: cache_dir
	cc -c ${src_dir}/camel_cards.c ${cflags} -I${include_dir} -o ${cache_dir}/camel_cards.o

cache_dir:
	mkdir -p ${cache_dir}

run: build
	./${out_file}

