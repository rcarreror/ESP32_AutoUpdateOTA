void createEmptyJson() {
  confJSON["configured"] = false;
  confJSON["nombreCliente"] = "";
  confJSON["nombreAsistente"] = "";
  confJSON["diaActual"] = 0;
  confJSON["idAsistente"] = "423526462";

  JsonArray pastillas = confJSON.createNestedArray("pastillas");

  JsonObject pastillas_0 = pastillas.createNestedObject();
  pastillas_0["nombrePastilla"] = "";
  pastillas_0["tipoPastilla"] = "";
  pastillas_0["frecuencia"] = 0;

  JsonArray pastillas_0_tomasNecesarias = pastillas_0.createNestedArray("tomasNecesarias");

  JsonArray pastillas_0_ultimasTomas = pastillas_0.createNestedArray("ultimasTomas");

  JsonObject pastillas_1 = pastillas.createNestedObject();
  pastillas_1["nombrePastilla"] = "";
  pastillas_1["tipoPastilla"] = "";
  pastillas_1["frecuencia"] = 0;

  JsonArray pastillas_1_tomasNecesarias = pastillas_1.createNestedArray("tomasNecesarias");

  JsonArray pastillas_1_ultimasTomas = pastillas_1.createNestedArray("ultimasTomas");

  JsonObject pastillas_2 = pastillas.createNestedObject();
  pastillas_2["nombrePastilla"] = "";
  pastillas_2["tipoPastilla"] = "";
  pastillas_2["frecuencia"] = 0;

  JsonArray pastillas_2_tomasNecesarias = pastillas_2.createNestedArray("tomasNecesarias");

  JsonArray pastillas_2_ultimasTomas = pastillas_2.createNestedArray("ultimasTomas");

  //EepromStream eepromStream(0, 1536);
  serializeJson(confJSON, eepromStream);
  eepromStream.flush();
}

void createTestJson() {
  confJSON["configured"] = true;
  confJSON["nombreCliente"] = "Paco Benancio Ramirez";
  confJSON["nombreAsistente"] = "Francisco Ramirez";
  confJSON["diaActual"] = 0;
  confJSON["idAsistente"] = "423526462";

  JsonArray pastillas = confJSON.createNestedArray("pastillas");

  JsonObject pastillas_0 = pastillas.createNestedObject();
  pastillas_0["nombrePastilla"] = "Ameride";
  pastillas_0["tipoPastilla"] = "antihipertensivo";
  pastillas_0["frecuencia"] = 8;

  JsonArray pastillas_0_tomasNecesarias = pastillas_0.createNestedArray("tomasNecesarias");
  pastillas_0_tomasNecesarias.add(6);
  pastillas_0_tomasNecesarias.add(14);
  pastillas_0_tomasNecesarias.add(22);

  JsonArray pastillas_0_ultimasTomas = pastillas_0.createNestedArray("ultimasTomas");
  pastillas_0_ultimasTomas.add("06:05");
  pastillas_0_ultimasTomas.add("14:10");
  pastillas_0_ultimasTomas.add("22:01");

  JsonObject pastillas_1 = pastillas.createNestedObject();
  pastillas_1["nombrePastilla"] = "Veratran";
  pastillas_1["tipoPastilla"] = "ansiolitico";
  pastillas_1["frecuencia"] = 12;

  JsonArray pastillas_1_tomasNecesarias = pastillas_1.createNestedArray("tomasNecesarias");
  pastillas_1_tomasNecesarias.add(6);
  pastillas_1_tomasNecesarias.add(18);

  JsonArray pastillas_1_ultimasTomas = pastillas_1.createNestedArray("ultimasTomas");
  pastillas_1_ultimasTomas.add("06:06");
  pastillas_1_ultimasTomas.add("18:05");

JsonObject pastillas_2 = pastillas.createNestedObject();
pastillas_2["nombrePastilla"] = "Tramadol";
pastillas_2["tipoPastilla"] = "analgesico";
pastillas_2["frecuencia"] = 12;

JsonArray pastillas_2_tomasNecesarias = pastillas_2.createNestedArray("tomasNecesarias");
pastillas_2_tomasNecesarias.add(6);
pastillas_2_tomasNecesarias.add(18);

JsonArray pastillas_2_ultimasTomas = pastillas_2.createNestedArray("ultimasTomas");
pastillas_2_ultimasTomas.add("06:03");
pastillas_2_ultimasTomas.add("18:00");

  //EepromStream eepromStream(0, 1536);
  serializeJson(confJSON, eepromStream);
  eepromStream.flush();
}
