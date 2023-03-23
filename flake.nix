{
  description = "Development environment with GCC and PROS";
  inputs = {
    nixpkgs.url = "github:NixOS/nixpkgs/nixpkgs-unstable";
  };
  outputs = { self, nixpkgs }: 
    let
      system = "x86_64-linux";
      
      pkgs = import nixpkgs { 
        inherit system;
      };

      python-package-list = p: with p; [
        pip
      ];
      pros-python = pkgs.python310.withPackages python-package-list;
    in
    {
    devShells.x86_64-linux.default = with pkgs; mkShell {
      buildInputs = [ pros-python gcc-arm-embedded ];
      shellHook = 
        ''
        python -m venv .venv
        source .venv/bin/activate
        pip install pros-cli
        '';
      
    };
  };
}