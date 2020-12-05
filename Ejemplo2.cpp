void ActivationMessages::sendActivationMessage(uint8_t byte1, uint16_t idObject, uint8_t state, uint8_t aux)
{
    QList<uint8_t> message;

    uint8_t sa = OPManage::getInstance()->list_object.at(OPManage::getInstance()->idContainerWS)->getName()->getSource_address();
    switch(byte1)
    {
        case 0:// Soft Key Activation Message
        {
            message.append(byte1);                                          //Command
            message.append(state);                                          // 0 released - 1 Pressed - 2 Still held - 3 Press aborted
            message.append(idObject & 0x00FF);                              //ID Object
            message.append((idObject & 0xFF00)>>8);                         //ID Object
            message.append(OPManage::getInstance()->activeMask);            //CAMBIAR
            message.append(OPManage::getInstance()->activeMask2);           //Key Group Object if the key is in a visible group
            message.append(aux);                                            //Soft Key Code/ 0 = ACK Alarm
            message.append(0xFF);
            //message.append(0xF | TAN << 4);                               //VT Version 6 and later send TAN (Transaction Number)
            //TAN++;
            CommunicationController::getInstance()->sendMessage(message, 0xE600, sa, true);
            MessagesToSend *m = new MessagesToSend(message, sa, 0xE600);
            sendMessagesList.append(m);
            break;
        }
        case 1:// Button Activation Message
        {
            message.append(byte1);                                          //Command
            message.append(state);                                          // 0 unlatched or released - 1 Pressed or latched - 2 Still held - 3 Press aborted
            message.append(idObject & 0x00FF);                              //ID Object
            message.append((idObject & 0xFF00)>>8);                         //ID Object
            message.append(OPManage::getInstance()->activeMask);            //CAMBIAR
            message.append(OPManage::getInstance()->activeMask2);           //Or Window Mask object
            message.append(aux);                                            //Button Key Code
            message.append(0xFF);
            //message.append(0xF | TAN << 4);                               //VT Version 6 and later send TAN (Transaction Number)
            //TAN++;
            CommunicationController::getInstance()->sendMessage(message, 0xE600, sa, true);
            MessagesToSend *m = new MessagesToSend(message, sa, 0xE600);
            sendMessagesList.append(m);
            break;
        }
        case 3://Select Input Object
        {
            message.append(byte1);                                          //Command
            message.append(idObject & 0x00FF);                              //ID Object
            message.append((idObject & 0xFF00)>>8);                         //ID Object
            message.append(state);                                          // 0 deselected - 1 selected
            message.append(aux);                                            //Bitmask. (Bit 0=1 Object is open for data input)
            message.append(0xFF);                                           //Reserved
            message.append(0xFF);                                           //Reserved
            message.append(0xFF);
            //message.append(0xF | TAN << 4);                               //VT Version 6 and later send TAN (Transaction Number)
            //TAN++;
            CommunicationController::getInstance()->sendMessage(message, 0xE600, sa, true);
            MessagesToSend *m = new MessagesToSend(message, sa, 0xE600);
            sendMessagesList.append(m);
            break;
        }
        case 4:// ESC message
        {
            message.append(byte1);                                          //Command
            message.append(idObject & 0x00FF);                              //ID Object
            message.append((idObject & 0xFF00)>>8);                         //ID Object
            message.append(state);                                          //Error Codes
            message.append(0xFF);
            message.append(0xFF);
            message.append(0xFF);
            message.append(0xFF);
            CommunicationController::getInstance()->sendMessage(message, 0xE600, sa, true);
            MessagesToSend *m = new MessagesToSend(message, sa, 0xE600);
            sendMessagesList.append(m);
            break;
        }
        case 6:// Change Active Mask message
        {
            message.append(byte1);                                          //Command
            message.append(OPManage::getInstance()->activeMask);            //Mask Object ID
            message.append(OPManage::getInstance()->activeMask2);           //Mask Object ID
            message.append(state);                                          //Error Codes
            message.append(idObject & 0x00FF);                              //ID Object
            message.append((idObject & 0xFF00)>>8);                         //ID Object
            message.append(0xFF);
            message.append(0xFF);
            CommunicationController::getInstance()->sendMessage(message, 0xE600, sa, true);
            MessagesToSend *m = new MessagesToSend(message, sa, 0xE600);
            sendMessagesList.append(m);
            break;
        }
        case 7:// Change Soft Key Mask message
        {
            message.append(byte1);                                          //Command
            message.append(OPManage::getInstance()->activeMask);            //Mask Object ID
            message.append(OPManage::getInstance()->activeMask2);           //Mask Object ID
            message.append(OPManage::getInstance()->activeSoftKey);         //Soft Key Mask ID
            message.append(OPManage::getInstance()->activeSoftKey2);        //Soft Key Mask ID
            message.append(state);                                          //Error Codes
            message.append(0xFF);
            message.append(0xFF);
            CommunicationController::getInstance()->sendMessage(message, 0xE600, sa, true);
            MessagesToSend *m = new MessagesToSend(message, sa, 0xE600);
            sendMessagesList.append(m);
            break;
        }
        case 0x09:// On User-Layout Hide/Show message
        {
            break;
        }
        case 0x0A:// Control Audio Signal Termination message
        {
            break;
        }
    }

}
/**************************************************************************************************************************/
/**
 * @brief ActivationMessages::sendActivationMessage. Pointing Event message
 */
void ActivationMessages::sendActivationMessage(uint8_t state, uint16_t x, uint16_t y)
{
    Q_UNUSED(state)
    QList<uint8_t> message;
    uint8_t sa = OPManage::getInstance()->list_object.at(OPManage::getInstance()->idContainerWS)->getName()->getSource_address();

    message.append(0x02);                                           //Command
    message.append((x & 0x00FF));                                   //X Position
    message.append((x & 0xFF00)>>8);                                //X Position
    message.append((y & 0x00FF));                                   //Y Position
    message.append((y & 0xFF00)>>8);                                //Y Position
    //message.append(state);                                          //0 released - 1 Pressed - 2 held
    message.append(0xFF);
    message.append(0xFF);
    message.append(0xFF);

//    message.append(OPManage::getInstance()->activeMask);            //VT 6 and later Parent Mask ID
//    message.append(OPManage::getInstance()->activeMask2);
    CommunicationController::getInstance()->sendMessage(message, 0xE600, sa, true);
    MessagesToSend *m = new MessagesToSend(message, sa, 0xE600);
    sendMessagesList.append(m);
}