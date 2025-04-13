arm-none-eabi-toolchain-dir ?= gcc-arm-none-eabi
arm-none-eabi-toolchain-temp-tar-file ?= ane.tar.bz2

.PHONY: build-with-local-toolchain
build-with-local-toolchain:
	mkdir -p build
	cd build
	cmake -S . -B ./build \
		-DCMAKE_C_COMPILER="$(arm-none-eabi-toolchain-dir)/bin/arm-none-eabi-gcc" \
		-DCMAKE_CPP_COMPILER="$(arm-none-eabi-toolchain-dir)/bin/arm-none-eabi-g++"
	make -C ./build -j4
	cd ..

.PHONY: download-arm-none-eabi-toolchain-macos
download-arm-none-eabi-toolchain-macos:
	wget -O $(arm-none-eabi-toolchain-temp-tar-file) 'https://developer.arm.com/-/media/Files/downloads/gnu-rm/10.3-2021.10/gcc-arm-none-eabi-10.3-2021.10-mac.tar.bz2?rev=58ed196feb7b4ada8288ea521fa87ad5&hash=62C9BE56E5F15D7C2D98F48BFCF2E839D7933597'
	tar -xvjf $(arm-none-eabi-toolchain-temp-tar-file)
	rm $(arm-none-eabi-toolchain-temp-tar-file)
	mv $$(find . -maxdepth 1 -type d -name "gcc-arm-none-eabi*") $(arm-none-eabi-toolchain-dir)

.PHONY: download-arm-none-eabi-toolchain-linux-x86_64
download-arm-none-eabi-toolchain-linux-x86_64:
	wget -O $(arm-none-eabi-toolchain-temp-tar-file) 'https://developer.arm.com/-/media/Files/downloads/gnu-rm/10.3-2021.10/gcc-arm-none-eabi-10.3-2021.10-x86_64-linux.tar.bz2?rev=78196d3461ba4c9089a67b5f33edf82a&hash=D484B37FF37D6FC3597EBE2877FB666A41D5253B'
	tar -xvjf $(arm-none-eabi-toolchain-temp-tar-file)
	rm $(arm-none-eabi-toolchain-temp-tar-file)
	mv $$(find . -maxdepth 1 -type d -name "gcc-arm-none-eabi*") $(arm-none-eabi-toolchain-dir)

.PHONY: download-arm-none-eabi-toolchain-linux-aarch64
download-arm-none-eabi-toolchain-linux-aarch64:
	wget -O $(arm-none-eabi-toolchain-temp-tar-file) 'https://developer.arm.com/-/media/Files/downloads/gnu-rm/10.3-2021.10/gcc-arm-none-eabi-10.3-2021.10-aarch64-linux.tar.bz2?rev=b748c39178c043b4915b04645d7774d8&hash=572217C8AFE83F1010753EA3E3A7EC2307DADD58'
	tar -xvjf $(arm-none-eabi-toolchain-temp-tar-file)
	rm $(arm-none-eabi-toolchain-temp-tar-file)
	mv $$(find . -maxdepth 1 -type d -name "gcc-arm-none-eabi*") $(arm-none-eabi-toolchain-dir)
