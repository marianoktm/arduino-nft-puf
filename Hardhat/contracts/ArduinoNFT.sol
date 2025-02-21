pragma solidity ^0.8.0;

import "@openzeppelin/contracts/token/ERC721/ERC721.sol";
import "@openzeppelin/contracts/access/Ownable.sol";

/**
 * @title ArduinoNFT
 * @dev This NFT contract represents ownership of Arduino devices.
 * Each device is uniquely identified by its SHA‑256 digest.
 * The digest (as bytes32) is cast to a uint256 and used as the tokenId.
 */
contract ArduinoNFT is ERC721, Ownable {
    constructor(address initialOwner) ERC721("Arduino Device PUF NFT", "PUFNFT") Ownable(initialOwner) {}

    /**
     * @notice Mints a new NFT representing an Arduino device.
     * @param to The address that will own the minted NFT.
     * @param deviceDigest The SHA‑256 digest (as bytes32) uniquely identifying the device.
     *
     * Requirements:
     * - Only the contract owner can call this function.
     * - The deviceDigest must not have been used to mint a token before.
     */
    function mint(address to, bytes32 deviceDigest) public onlyOwner {
        uint256 tokenId = uint256(deviceDigest);
        _safeMint(to, tokenId);
    }

    /**
     * @notice Returns the owner of the device represented by its SHA‑256 digest.
     * @param deviceDigest The SHA‑256 digest (as bytes32) uniquely identifying the device.
     * @return address The owner of the device NFT.
     *
     * Requirements:
     * - The device must have been minted.
     */
    function ownerOfDevice(bytes32 deviceDigest) public view returns (address) {
        uint256 tokenId = uint256(deviceDigest);
        return ownerOf(tokenId);
    }
}
