{
  inputs = {};
  outputs = {
    devShells.default = with pkgs; mkShell {
      buildInputs = [ python310 gcc-arm-embedded ];
      shellHook = 
        ''
        source enviro/bin/activate
        pros mut
        '';
      
    };
  };
}