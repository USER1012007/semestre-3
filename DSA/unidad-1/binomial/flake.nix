{
  description = "C++ Qt6 Towers of Hanoi Development Environment";

  inputs = {
    nixpkgs.url = "github:nixos/nixpkgs/nixos-unstable";
  };

  outputs =
    { self, nixpkgs }:
    let
      supportedSystems = [
        "x86_64-linux"
        "aarch64-linux"
      ];
      forEachSupportedSystem =
        f:
        nixpkgs.lib.genAttrs supportedSystems (
          system:
          f {
            pkgs = import nixpkgs { inherit system; };
          }
        );
    in
    {
      devShells = forEachSupportedSystem (
        { pkgs }: {
          default = pkgs.mkShell {
            packages = with pkgs; [
              gcc13
              cmake
              gnumake

              qt6.qtbase
              qt6.wrapQtAppsHook
            ];

            shellHook = ''
              export QT_QPA_PLATFORM=wayland
              export QT_QPA_PLATFORMTHEME=qt6ct
            '';
          };
        }
      );
    };
}
