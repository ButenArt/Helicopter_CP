#include "Helicopter_canwindow.h"
#include "ui_Helicopter_CanWindow.h"
#include "Helicopter_controls.h"
#include "Common/log_duration.h"

Helicopter_CANWindow::Helicopter_CANWindow(QWidget *parent, QSharedPointer<Helicopter_controls> controls )
    : QWidget(parent)
    , ui(new Ui::Helicopter_CANWindow)
{
    ui->setupUi( this );
    this->setWindowFlags( Qt::Window | Qt::WindowCloseButtonHint );
    this->setFixedSize( this->width( ), this->height( ) );
    this->setWindowTitle( "CAN" );
    QEvent::UpdateLater; // <-кто нибудь понимает, что это такое?
    Qt::LowEventPriority;

    _controls = controls;

    UpdateTimer = new QTimer( this );
    UpdateTimer->setInterval( 200 );
    connect( UpdateTimer, SIGNAL( timeout( ) ), this, SLOT( slotParamsUpdate( ) ) );
    UpdateTimer->stop( );

    for ( int i = 0; i < N_PARAM_MAB; i++ )
    {
        ui->lst_MAB->addItem( "[" + _controls->GetMABCaption( i ) + ": " + QString::number( _controls->GetMABNormalValue( i ) ) + "] [MAB: " + QString::number( _controls->GetMABValue( i ) ) + "]" + _controls->GetMABaddr( i ) );
    }
    for ( int i = 0; i < N_PARAM_VID; i++ )
    {
        ui->lst_VID->addItem( "[" + _controls->GetVIDCaption( i ) + ": " + QString::number( _controls->GetVIDValue( i ) ) + "] [Steps: " + QString::number( _controls->GetVIDSteps( i ) ) + "]" + _controls->GetVIDaddr( i ) );
    }
    for ( int i = 0; i < N_PARAM_CLOCK; i++ )
    {
        ui->lst_CLOCK->addItem( "[" + _controls->GetCLOCKCaption( i ) + ": " + QString::number( _controls->GetCLOCKValue( i ) ) + "] [Steps: " + QString::number( _controls->GetCLOCKSteps( i ) ) + "]" + _controls->GetCLOCKaddr( i ) );
    }

    ui->lst_IN1->addItem( "IN0 [0x" + QString::number( _controls->GetGPIOINaddr( 0 ), 16 ).toUpper( ).rightJustified( 3, '0' ) + "] " + _controls->BinView( _controls->GetGPIOIN( 0 ) ) );
    ui->lst_IN1->addItem( "IN1 [0x" + QString::number( _controls->GetGPIOINaddr( 1 ), 16 ).toUpper( ).rightJustified( 3, '0' ) + "] " + _controls->BinView( _controls->GetGPIOIN( 1 ) ) );
    ui->lst_IN1->addItem( "IN2 [0x" + QString::number( _controls->GetGPIOINaddr( 2 ), 16 ).toUpper( ).rightJustified( 3, '0' ) + "] " + _controls->BinView( _controls->GetGPIOIN( 2 ) ) );
    ui->lst_IN1->addItem( "IN3 [0x" + QString::number( _controls->GetGPIOINaddr( 3 ), 16 ).toUpper( ).rightJustified( 3, '0' ) + "] " + _controls->BinView( _controls->GetGPIOIN( 3 ) ) );
    ui->lst_IN1->addItem( "IN4 [0x" + QString::number( _controls->GetGPIOINaddr( 4 ), 16 ).toUpper( ).rightJustified( 3, '0' ) + "] " + _controls->BinView( _controls->GetGPIOIN( 4 ) ) );
    ui->lst_IN1->addItem( "IN5 [0x" + QString::number( _controls->GetGPIOINaddr( 5 ), 16 ).toUpper( ).rightJustified( 3, '0' ) + "] " + _controls->BinView( _controls->GetGPIOIN( 5 ) ) );
    ui->lst_IN1->addItem( "IN6 [0x" + QString::number( _controls->GetGPIOINaddr( 6 ), 16 ).toUpper( ).rightJustified( 3, '0' ) + "] " + _controls->BinView( _controls->GetGPIOIN( 6 ) ) );
    ui->lst_IN1->addItem( "IN7 [0x" + QString::number( _controls->GetGPIOINaddr( 7 ), 16 ).toUpper( ).rightJustified( 3, '0' ) + "] " + _controls->BinView( _controls->GetGPIOIN( 7 ) ) );
    ui->lst_OUT1->addItem( "OUT0 [0x" + QString::number( _controls->GetGPIOOUTaddr( 0 ), 16 ).toUpper( ).rightJustified( 3, '0' ) + "] " + _controls->BinView( _controls->GetGPIOOUT( 0 ) ) );
    ui->lst_OUT1->addItem( "OUT1 [0x" + QString::number( _controls->GetGPIOOUTaddr( 1 ), 16 ).toUpper( ).rightJustified( 3, '0' ) + "] " + _controls->BinView( _controls->GetGPIOOUT( 1 ) ) );
    ui->lst_OUT1->addItem( "OUT2 [0x" + QString::number( _controls->GetGPIOOUTaddr( 2 ), 16 ).toUpper( ).rightJustified( 3, '0' ) + "] " + _controls->BinView( _controls->GetGPIOOUT( 2 ) ) );
    ui->lst_OUT1->addItem( "OUT3 [0x" + QString::number( _controls->GetGPIOOUTaddr( 3 ), 16 ).toUpper( ).rightJustified( 3, '0' ) + "] " + _controls->BinView( _controls->GetGPIOOUT( 3 ) ) );
    ui->lst_OUT1->addItem( "OUT4 [0x" + QString::number( _controls->GetGPIOOUTaddr( 4 ), 16 ).toUpper( ).rightJustified( 3, '0' ) + "] " + _controls->BinView( _controls->GetGPIOOUT( 4 ) ) );
    ui->lst_OUT1->addItem( "OUT5 [0x" + QString::number( _controls->GetGPIOOUTaddr( 5 ), 16 ).toUpper( ).rightJustified( 3, '0' ) + "] " + _controls->BinView( _controls->GetGPIOOUT( 5 ) ) );
    ui->lst_OUT1->addItem( "OUT6 [0x" + QString::number( _controls->GetGPIOOUTaddr( 6 ), 16 ).toUpper( ).rightJustified( 3, '0' ) + "] " + _controls->BinView( _controls->GetGPIOOUT( 6 ) ) );
    ui->lst_OUT1->addItem( "OUT7 [0x" + QString::number( _controls->GetGPIOOUTaddr( 7 ), 16 ).toUpper( ).rightJustified( 3, '0' ) + "] " + _controls->BinView( _controls->GetGPIOOUT( 7 ) ) );
    for ( int i = 0; i < N_PARAM_POT; i++ )
    {
        ui->lst_POTENTIOMETER1->addItem( "[" + _controls->GetPOTCaption( i ) + ": " + QString::number( _controls->GetPOTNormalValue( i ) ) + "] [POT: " + QString::number( _controls->GetPOTValue( i ) ) + "]" + _controls->GetPOTaddr( i ) );
    }
    T_UNITS units = _controls->GetUnitsData( );
    for ( auto& unit : units.MAB ) {
        ui->lst_Units->addItem( unit.INFO );
    }
    for ( auto& unit : units.VID ) {
        ui->lst_Units->addItem( unit.INFO );
    }
    for ( auto& unit : units.GPIN ) {
        ui->lst_Units->addItem( unit.INFO );
    }
    for ( auto& unit : units.GPOUT ) {
        ui->lst_Units->addItem( unit.INFO );
    }
    for ( auto& unit : units.ENC ) {
        ui->lst_Units->addItem( unit.INFO );
    }
    for ( auto& unit : units.POT ) {
        ui->lst_Units->addItem( unit.INFO );
    }
}

Helicopter_CANWindow::~Helicopter_CANWindow()
{
    delete UpdateTimer;
    delete ui;
}

void Helicopter_CANWindow::closeEvent(QCloseEvent *event)
{
    event->ignore();
    UpdateTimer->stop();
    this->hide();
}

void Helicopter_CANWindow::slotParamsUpdate( )
{
    {
        //LOG_DURATION( __FUNCTION__ );
        {
            //LOG_DURATION( "MAB" );
            for ( int i = 0; i < N_PARAM_MAB; i++ )
            {
                {
                    ui->lst_MAB->item( i )->setText( "[" + _controls->GetMABCaption( i ) + ": " + QString::number( _controls->GetMABNormalValue( i ) ) + "] [MAB: " + QString::number( _controls->GetMABValue( i ) ) + "]" + _controls->GetMABaddr( i ) );
                }
                if ( _controls->MABOutOfRange( i ) ) ui->lst_MAB->item( i )->setForeground( Qt::red ); else ui->lst_MAB->item( i )->setForeground( Qt::black );
            }
        }

        {
            //LOG_DURATION( "VID set" );
            for ( int i = 0; i < N_PARAM_VID; i++ )
            {
                ui->lst_VID->item( i )->setText( "[" + _controls->GetVIDCaption( i ) + ": " + QString::number( _controls->GetVIDValue( i ) ) + "] [Steps: " + QString::number( _controls->GetVIDSteps( i ) ) + "]" + _controls->GetVIDaddr( i ) );
            }
        }
        {
            //LOG_DURATION( "CLOCK set" );
            for ( int i = 0; i < N_PARAM_CLOCK; i++ )
            {
                ui->lst_CLOCK->item( i )->setText( "[" + _controls->GetCLOCKCaption( i ) + ": " + QString::number( _controls->GetCLOCKValue( i ) ) + "] [Steps: " + QString::number( _controls->GetCLOCKSteps( i ) ) + "]" + _controls->GetCLOCKaddr( i ) );
            }
        }

        {
            //LOG_DURATION( "IN" );
            ui->lst_IN1->item( 0 )->setText( "IN0 [0x" + QString::number( _controls->GetGPIOINaddr( 0 ), 16 ).rightJustified( 3, '0' ) + "] " + _controls->BinView( _controls->GetGPIOIN( 0 ) ) );
            ui->lst_IN1->item( 1 )->setText( "IN1 [0x" + QString::number( _controls->GetGPIOINaddr( 1 ), 16 ).rightJustified( 3, '0' ) + "] " + _controls->BinView( _controls->GetGPIOIN( 1 ) ) );
            ui->lst_IN1->item( 2 )->setText( "IN2 [0x" + QString::number( _controls->GetGPIOINaddr( 2 ), 16 ).rightJustified( 3, '0' ) + "] " + _controls->BinView( _controls->GetGPIOIN( 2 ) ) );
            ui->lst_IN1->item( 3 )->setText( "IN3 [0x" + QString::number( _controls->GetGPIOINaddr( 3 ), 16 ).rightJustified( 3, '0' ) + "] " + _controls->BinView( _controls->GetGPIOIN( 3 ) ) );
            ui->lst_IN1->item( 4 )->setText( "IN4 [0x" + QString::number( _controls->GetGPIOINaddr( 4 ), 16 ).rightJustified( 3, '0' ) + "] " + _controls->BinView( _controls->GetGPIOIN( 4 ) ) );
            ui->lst_IN1->item( 5 )->setText( "IN5 [0x" + QString::number( _controls->GetGPIOINaddr( 5 ), 16 ).rightJustified( 3, '0' ) + "] " + _controls->BinView( _controls->GetGPIOIN( 5 ) ) );
            ui->lst_IN1->item( 6 )->setText( "IN6 [0x" + QString::number( _controls->GetGPIOINaddr( 6 ), 16 ).rightJustified( 3, '0' ) + "] " + _controls->BinView( _controls->GetGPIOIN( 6 ) ) );
            ui->lst_IN1->item( 7 )->setText( "IN7 [0x" + QString::number( _controls->GetGPIOINaddr( 7 ), 16 ).rightJustified( 3, '0' ) + "] " + _controls->BinView( _controls->GetGPIOIN( 7 ) ) );
        }

        {
            //LOG_DURATION( "OUT" );
            ui->lst_OUT1->item( 0 )->setText( "OUT0 [0x" + QString::number( _controls->GetGPIOOUTaddr( 0 ), 16 ).rightJustified( 3, '0' ) + "] " + _controls->BinView( _controls->GetGPIOOUT( 0 ) ) );
            ui->lst_OUT1->item( 1 )->setText( "OUT1 [0x" + QString::number( _controls->GetGPIOOUTaddr( 1 ), 16 ).rightJustified( 3, '0' ) + "] " + _controls->BinView( _controls->GetGPIOOUT( 1 ) ) );
            ui->lst_OUT1->item( 2 )->setText( "OUT2 [0x" + QString::number( _controls->GetGPIOOUTaddr( 2 ), 16 ).rightJustified( 3, '0' ) + "] " + _controls->BinView( _controls->GetGPIOOUT( 2 ) ) );
            ui->lst_OUT1->item( 3 )->setText( "OUT3 [0x" + QString::number( _controls->GetGPIOOUTaddr( 3 ), 16 ).rightJustified( 3, '0' ) + "] " + _controls->BinView( _controls->GetGPIOOUT( 3 ) ) );
            ui->lst_OUT1->item( 4 )->setText( "OUT4 [0x" + QString::number( _controls->GetGPIOOUTaddr( 4 ), 16 ).rightJustified( 3, '0' ) + "] " + _controls->BinView( _controls->GetGPIOOUT( 4 ) ) );
            ui->lst_OUT1->item( 5 )->setText( "OUT5 [0x" + QString::number( _controls->GetGPIOOUTaddr( 5 ), 16 ).rightJustified( 3, '0' ) + "] " + _controls->BinView( _controls->GetGPIOOUT( 5 ) ) );
            ui->lst_OUT1->item( 6 )->setText( "OUT6 [0x" + QString::number( _controls->GetGPIOOUTaddr( 6 ), 16 ).rightJustified( 3, '0' ) + "] " + _controls->BinView( _controls->GetGPIOOUT( 6 ) ) );
            ui->lst_OUT1->item( 7 )->setText( "OUT7 [0x" + QString::number( _controls->GetGPIOOUTaddr( 7 ), 16 ).rightJustified( 3, '0' ) + "] " + _controls->BinView( _controls->GetGPIOOUT( 7 ) ) );
        }
        {
            //LOG_DURATION( "POT" );
            for ( int i = 0; i < N_PARAM_POT; i++ )
            {
                ui->lst_POTENTIOMETER1->item( i )->setText( "[" + _controls->GetPOTCaption( i ) + ": " + QString::number( _controls->GetPOTNormalValue( i ) ) + "] [POT: " + QString::number( _controls->GetPOTValue( i ) ) + "]" + _controls->GetPOTaddr( i ) );
                if ( _controls->POTOutOfRange( i ) )
                    ui->lst_POTENTIOMETER1->item( i )->setForeground( Qt::red );
                else
                    ui->lst_POTENTIOMETER1->item( i )->setForeground( Qt::black );
            }
        }

        {
            //LOG_DURATION( "UNIT" );
            T_UNITS units = _controls->GetUnitsData( );
            int row = 0;
            for ( auto& unit : units.MAB ) {
                QListWidgetItem* item = ui->lst_Units->item( row++ );
                item->setText( unit.INFO );
            }
            for ( auto& unit : units.VID ) {
                QListWidgetItem* item = ui->lst_Units->item( row++ );
                item->setText( unit.INFO );
            }
            for ( auto& unit : units.GPIN ) {
                QListWidgetItem* item = ui->lst_Units->item( row++ );
                item->setText( unit.INFO );
            }
            for ( auto& unit : units.GPOUT ) {
                QListWidgetItem* item = ui->lst_Units->item( row++ );
                item->setText( unit.INFO );
            }
            for ( auto& unit : units.ENC ) {
                QListWidgetItem* item = ui->lst_Units->item( row++ );
                item->setText( unit.INFO );
            }
            for ( auto& unit : units.POT ) {
                QListWidgetItem* item = ui->lst_Units->item( row++ );
                item->setText( unit.INFO );
            }
        }
    }
}
