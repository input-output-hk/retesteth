{ lib, stdenv, fetchFromGitHub, cmake }:

stdenv.mkDerivation rec {
  pname = "libscrypt";
  version = "0.0.1";

  src = fetchFromGitHub {
    owner = "hunter-packages";
    repo = pname;
    rev = "62755c372cdcb8e40f35cf779f3abb045aa39063";
    sha256 = "sha256-UBRiSG4VFiAADEMiK1klmH/RwL0y/ZLvA1DNaAk5U1o=";
  };

  nativeBuildInputs = [ cmake ];
}
