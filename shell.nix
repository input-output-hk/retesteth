with import <nixpkgs> { };

let
  cryptoPcFile = writeTextFile {
    name = "libcryptopp.pc";
    text = ''
      # Crypto++ package configuration file
      prefix=@out@
      libdir=''${prefix}/lib
      includedir=@dev@/include

      Name: Crypto++
      Description: Crypto++ cryptographic library
      Version: 5.6.5
      URL: https://cryptopp.com/

      Cflags: -I''${includedir}
      Libs: -L''${libdir} -lcryptopp
    '';
  };
  cryptopp_5_6_5 = cryptopp.overrideAttrs( oldAttrs: {
      src = fetchFromGitHub {
        owner = "weidai11";
        repo = "cryptopp";
        rev = "CRYPTOPP_5_6_5";
        sha256 = "sha256-h+7LK8nzk1NlkVB4Loc9VQpN79SUFvBYESSpTZyXZ/o=";
      };
      postPatch = '''';
      preConfigure = '' '';
      buildFlags = [ "static" "shared" ];
      installTargets = "";
      postInstall = ''
        mkdir -p $dev/lib/pkgconfig
        substituteAll ${cryptoPcFile} $dev/lib/pkgconfig/libcryptopp.pc
        ln -s $out/lib/libcryptopp.so $out/lib/libcryptopp.so.5.6
      '';
    });

  secp256k1_old = (secp256k1.overrideAttrs (oldAttrs: {
      src = fetchurl {
        url = "https://github.com/chfast/secp256k1/archive/ac8ccf29b8c6b2b793bc734661ce43d1f952977a.tar.gz";
        sha256 = "02f8f05c9e9d2badc91be8e229a07ad5e4984c1e77193d6b00e549df129e7c3a";
      };
      preConfigure = ''
        alias cc=$CC
      '';
    })).override( {
      enableECDH = true;
    });
  hunter_libscrypt = callPackage ./hunter_libscrypt.nix {};
in
  {shell =
mkShell {
  nativeBuildInputs = [ cmake ];
  buildInputs = [
    cmake
    pkg-config
    boost175
    libyamlcpp
    hunter_libscrypt
    curl
    cryptopp_5_6_5
    secp256k1_old
  ];
};
crypto = cryptopp_5_6_5;
}
