{ pkgs ? import <nixpkgs> {} }:
  pkgs.mkShell rec {
    buildInputs = with pkgs; [
      python310
      gcc-arm-embedded
      git
    ];
    shellHook = 
    ''
    source enviro/bin/activate
    pros mut
    '';
  }