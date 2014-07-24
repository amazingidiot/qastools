
all: debug release

debug:
	cd ~/build/qastools/debug && make -j8

release:
	cd ~/build/qastools/release && make -j8

