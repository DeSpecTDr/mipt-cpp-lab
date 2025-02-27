{
  inputs.nixpkgs.url = "github:NixOS/nixpkgs/nixpkgs-unstable";
  outputs = {
    self,
    nixpkgs,
  }: let
    lib = nixpkgs.lib;
    system = "x86_64-linux";
    pkgs = nixpkgs.legacyPackages.${system};
    stdenv = pkgs.fastStdenv;
    # stdenv = pkgs.stdenvAdapters.useMoldLinker pkgs.fastStdenv; # profile guided gcc with mold linker
    # stdenv = stdenvAdapters.useMoldLinker llvmPackages_latest.stdenv;
    llvm = pkgs.llvmPackages;
    path = "/home/user/Projects/cpp";
    cachestd = pkgs.writeShellScriptBin "cachestd" ''
      g++ -xc++-header -O2 -std=c++20 -fsanitize=address -fsanitize=undefined -fno-sanitize-recover -D_GLIBCXX_DEBUG -D_GLIBCXX_DEBUG_PEDANTIC -D_FORTIFY_SOURCE=3 -D_GLIBCXX_ASSERTIONS -ftrapv -fstrict-flex-arrays=3 -fstack-clash-protection -fstack-protector-strong -fcf-protection=full -o ${path}/pch/bits/stdc++.h.gch \
        ${stdenv.cc.cc}/include/c++/*/x86_64-unknown-linux-gnu/bits/stdc++.h
    '';
  in {
    packages.${system}.cachestd = cachestd;
    devShells.${system}.default = pkgs.mkShell.override {inherit stdenv;} {
      nativeBuildInputs = with pkgs; let
        mymake = pkgs.writeShellScriptBin "mk" ''
          exec bash ${path}/mk.sh "$@"
        '';
        py = pkgs.python3.withPackages (ps:
          with ps; [
            jupyter
            ipykernel

            matplotlib
            numpy
            scipy
          ]);
      in [
        mymake
        # cachestd

        cppcheck
        valgrind

        gdb

        pkg-config
        meson
        ninja
        cmake
        gnumake

        llvm.clang-tools
        llvm.lldb
        llvm.bintools
        # llvm.clang
        # llvm.libcxxClang
        # llvm.libcxx
        # llvm.libllvm

        # mold-wrapped

        py
      ];

      buildInputs = with pkgs; [
        # boost184
        # nlohmann_json
      ];

      # CC_LIB_PATH = "${stdenv.cc.cc}";
    };
  };
}
