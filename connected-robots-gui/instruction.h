#ifndef INSTRUCTION_H
#define INSTRUCTION_H

#include <QString>
#include <QJsonDocument>

struct Position {
    double x = 0.0;
    double y = 0.0;
    double z = 0.0;
    double a = 0.0;
    double b = 0.0;
    double c = 0.0;
};

class Instruction {
private:
    double bracketOperatorDefaultValue;
public:
    QString instruction = "NOP"; // Standardwert "Keine Operation"
    double value = 0.0;
    Position pos1;
    Position pos2;
    Position pos3;

    /**
     * @brief Erstellt ein Instruction-Objekt mit Standardwerten
     * @details Der Standardwert ist bei allen Zahlenwerten 0.0
     *          und für den Member instruction "NOP"
     */
    Instruction() {};

    /**
     * @brief Erstellt ein Instruction-Objekt aus dem übergebenen JSON-Dokument
     * @param json JSON-Abbild einer Instruction
     *
     * @details Dieser Konstruktor ermöglicht es, ein Instruction-Objekt aus JSON-Daten zu erstellen,
     *          indem er die Werte aus dem JSON-Dokument liest und in die entsprechenden Felder des
     *          Instruction-Objekts schreibt.
     *          Das JSON-Dokument kann zum Beispiel mit Instruction::toJsonDoc() erzeugt werden.
     */
    Instruction(const QJsonDocument &jsonDoc);

    /**
     * @brief Wandelt das Instruction-Objekt in ein QJsonDocument um
     * @return Instruction als QJsonDocument
     */
    const QJsonDocument toJsonDoc();

    /**
     * @brief Wandelt das Instruction-Objekt in ein Json-String um
     * @param format gibt an, ob der String leserlich mit Umbrüchen und Einrückungen erstellt werden soll,
     *        oder alles in einer Zeile. Gültige Werte sind QJsonDocument::Compact oder QJsonDocument::Indented.
     *        Der Standardwert ist QJsonDocument::Indented.
     * @return Instruction als QString
     */
    const QString toJsonString(QJsonDocument::JsonFormat format = QJsonDocument::Indented);

    /**
     * @brief operator [] ermöglicht den Zugriff auf die Membervariabeln mittels Angabe eines String als Key
     * @param key muss einer der folgenden Strings sein: "pos1_x", "pos1_y", "pos1_z", "pos1_a", "pos1_b", "pos1_c", "pos2_x", "pos2_y", "pos2_z", "pos2_a", "pos2_b", "pos2_c", "pos3_x", "pos3_y", "pos3_z", "pos3_a", "pos3_b", "pos3_c"
     * @details operator [] ermöglicht den Zugriff auf die Membervariabeln mittels Angabe eines String als Key.
     *          Es kann nur auf Member zugegriffen werden, die als Datentyp double haben. Dies schließt nur Instruction::instruction aus.
     *          Falls ein String übergeben wird, der nicht bekannt ist, wird Instruction::bracketOperatorDefaultValue zurückgegeben.
     * @return Membervariable der Instruction
     */
    double& operator[](const QString& key);
};

#endif // INSTRUCTION_H
