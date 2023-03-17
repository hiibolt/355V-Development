{
  inputs = {
    nixpkgs.url = "github:NixOS/nixpkgs/nixpkgs-unstable";
  };
  outputs = { self, nixpkgs }: 
    let
      pkgs = import nixpkgs { 
        system = "x86_64-linux";
      };
    in
    {
    devShells.x86_64-linux.default = with pkgs; mkShell {
      buildInputs = [ python310 gcc-arm-embedded ];
      shellHook = 
        ''
        source enviro/bin/activate
        '';
      
    };
  };
}