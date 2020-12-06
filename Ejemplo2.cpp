bool ControllerVT::tryTakeFreeDir()
{
    if (timerAddress.elapsed() > (500 + ((qrand() % 256 + 0) * 0.6)))
    {
        if (address[PREFERED_ADDRESS] == 0) //Try take prefered address if this is free
        {
            CommunicationController::getInstance()->setidVT(PREFERED_ADDRESS);
        }
        else if (self_configurable_address == 0x01) //if we are configurable device, try take a free direction
        {
            for (uint8_t i = 128L; i < 248L; i++)
            {
                if (address[i] != 1)
                {
                    CommunicationController::getInstance()->setidVT(i);
                    i = 250;
                }
            }
        }
        if ((CommunicationController::getInstance()->getidVT() == 0xFE)) //If there aren´t a free directions try to steal it from another device
        {
            if (tryTakeDirByPriority() == false) // If we don´t have more priority than any device, we wait 8s and repeat process
            {
                this->sleep(3);
                askAddress();
                std::fill(address, address + 255, 0); //reset vector
                nameList.clear();                     //reset NAME table
                timerAddress.restart();               // reset timer
                return false;
            }
            return true;
        }
        return true;
    }
    return false;
}
void ControllerVT::queuedMessages(QList<uint8_t> message_list)
{
    this->freePositionSend.acquire();
    sendMessagesBufferVT[writeBufferPosSend % INPUT_BUFFER_SIZE] = message_list;
    writeBufferPosSend++;
    this->usedPositionSend.release();
}
/**************************************************************************************************************************/
/**
 * @brief ControllerVT::findSourceAddress. Devuelve la posicion del NAME en la lista de nombres
 * @param name
 * @return
 */
uint8_t ControllerVT::findSourceAddress(Name *name)
{
    for (int i = 0; i < this->nameList.size(); i++)
    {
        if (nameList.at(i)->comparator(name)) //Are equal
        {
            return nameList.at(i)->getSource_address();
        }
    }
    return 0xFE;
}
/**************************************************************************************************************************/
/**
 * @brief ControllerVT::tryTakeDirByPriority. Funcion para tomar la direccion de otro dispositivo si el VT tiene mas prioridad
 * @return
 */
bool ControllerVT::tryTakeDirByPriority()
{
    float64_t myName = calculateMyPriority();
    for (int i = 0; i < nameList.size(); i++)
    {
        if (nameList.at(i)->getSource_address() != PREFERED_ADDRESS && nameList.at(i)->getSource_address() > 127 && nameList.at(i)->getSource_address() < 247)
        {
            float64_t deviceName = static_cast<unsigned long long>(nameList.at(i)->getbyte8()) | static_cast<unsigned long long>(nameList.at(i)->getbyte7()) << 8 | static_cast<unsigned long long>(nameList.at(i)->getbyte6()) << 16 | static_cast<unsigned long long>(nameList.at(i)->getbyte5()) << 24 | static_cast<unsigned long long>(nameList.at(i)->getbyte4()) << 32 | static_cast<unsigned long long>(nameList.at(i)->getbyte3()) << 40 | static_cast<unsigned long long>(nameList.at(i)->getbyte2()) << 48 | static_cast<unsigned long long>(nameList.at(i)->getbyte1()) << 56;
            if (myName < deviceName) //VT have more priority
            {
                CommunicationController::getInstance()->setidVT(nameList.at(i)->getSource_address()); //Send address
                i = nameList.size();
                return true;
            }
        }
    }
    return false;
}