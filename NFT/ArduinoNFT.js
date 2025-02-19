const { buildModule } = require("@nomicfoundation/hardhat-ignition/modules");

module.exports = buildModule("ArduinoNFT", (m) => {
  const owner = m.getAccount(0);
  
  const pufnft = m.contract("ArduinoNFT", [owner]);

  return { pufnft };
});
