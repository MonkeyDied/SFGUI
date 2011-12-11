#include <SFGUI/SFGUI.hpp>
#include <SFGUI/Engines/BREW.hpp>

#include <SFML/Graphics.hpp>
#include <SFML/System/Clock.hpp>
#include <sstream>

class SampleApp {
	public:
		SampleApp();
		void Run();

	private:
		void OnAddButtonHClick();
		void OnAddButtonVClick();
		void OnToggleTitlebarClick();
		void OnHideWindowClicked();
		void OnToggleSpaceClick();
		void OnLimitCharsToggle();
		void OnLoadThemeClick();
		void OnToggleCullingClick();
		void OnAdjustmentChange();
		void OnToggleSpinner();

		sfg::Window::Ptr m_wndmain;
		sfg::Box::Ptr m_boxbuttonsh;
		sfg::Box::Ptr m_boxbuttonsv;
		sfg::Entry::Ptr m_entry;
		sfg::Table::Ptr m_table;
		sfg::ScrolledWindow::Ptr m_scrolled_window;
		sfg::Box::Ptr m_scrolled_window_box;
		sfg::ToggleButton::Ptr m_titlebar_toggle;
		sfg::CheckButton::Ptr m_limit_check;
		sfg::Scale::Ptr m_scale;
		sfg::ProgressBar::Ptr m_progress;
		sfg::ProgressBar::Ptr m_progress_vert;
		sfg::Spinner::Ptr m_spinner;

		sfg::Desktop m_desktop;

		unsigned int m_fps_counter;
		sf::Clock m_fps_clock;
		bool m_cull;

		sf::Texture m_background_texture;
		sf::Sprite m_background_sprite;

		sf::RenderWindow m_window;
};

class Ouchy : public sfg::EnableSharedFromThis<Ouchy> {
	public:
		typedef sfg::SharedPtr<Ouchy> Ptr; //!< Shared pointer.
		Ouchy( sfg::Button::Ptr button );

		void DoOuch();

		static std::vector<Ptr> m_ouchies;
	private:
		sfg::Button::Ptr m_button;
		bool m_state;
};

std::vector<Ouchy::Ptr> Ouchy::m_ouchies;

Ouchy::Ouchy( sfg::Button::Ptr button ) :
	m_button( button ),
	m_state( false )
{
}

void Ouchy::DoOuch() {
	if( !m_state ) {
		m_button->SetLabel( "Ouch" );
	}
	else {
		m_button->SetLabel( "Boom" );
	}

	m_state = !m_state;
}

SampleApp::SampleApp() :
	m_desktop( sf::FloatRect( .0f, .0f, 1024.f, 768.f ) ),
	m_cull( true ),
	m_window( sf::VideoMode( 1024, 768, 32 ), "SFGUI test" )
{
	m_background_texture.Create( 1024, 768 );

	sf::Uint8* pixels = new sf::Uint8[ 1024 * 768 * 4 ];

	sf::Uint8 pixel_value = 139;

	for( std::size_t index = 0; index < 1024 * 768; ++index ) {
		pixel_value = static_cast<sf::Uint8>( pixel_value ^ ( index + 809 ) );
		pixel_value = static_cast<sf::Uint8>( pixel_value << ( index % 11 ) );
		pixel_value = static_cast<sf::Uint8>( pixel_value * 233 );

		pixels[ index * 4 + 0 ] = static_cast<sf::Uint8>( pixel_value % 16 + 72 ); // R

		pixel_value ^= static_cast<sf::Uint8>( index );
		pixel_value = static_cast<sf::Uint8>( pixel_value * 23 );

		pixels[ index * 4 + 1 ] = static_cast<sf::Uint8>( pixel_value % 16 + 72 ); // G

		pixel_value ^= static_cast<sf::Uint8>( index );
		pixel_value = static_cast<sf::Uint8>( pixel_value * 193 );

		pixels[ index * 4 + 2 ] = static_cast<sf::Uint8>( pixel_value % 16 + 72 ); // B

		pixels[ index * 4 + 3 ] = 255; // A
	}

	m_background_texture.Update( pixels );

	m_background_sprite.SetTexture( m_background_texture );

	delete[] pixels;
}

void SampleApp::Run() {
	sf::Event event;

	//m_window.SetFramerateLimit( 60 );
	//m_window.EnableVerticalSync( true );

	// Create widgets.
	m_wndmain = sfg::Window::Create();
	m_wndmain->SetTitle( L"Example application" );

	sfg::Button::Ptr btnaddbuttonh( sfg::Button::Create( L"Add button horizontally" ) );
	sfg::Button::Ptr btnaddbuttonv( sfg::Button::Create( L"Add button vertically" ) );
	m_titlebar_toggle = sfg::ToggleButton::Create( "Toggle titlebar" );

	{
		sf::Image add_image;
		if( add_image.LoadFromFile( "data/add.png" ) ) {
			sfg::Image::Ptr image( sfg::Image::Create( add_image ) );
			btnaddbuttonh->SetImage( image );

			image = sfg::Image::Create( add_image );
			btnaddbuttonv->SetImage( image );
		}
	}

	sfg::Button::Ptr btnhidewindow( sfg::Button::Create( L"Close window" ) );
	btnhidewindow->SetId( "close" );

	{
		sf::Image close_image;
		if( close_image.LoadFromFile( "data/delete.png" ) ) {
			sfg::Image::Ptr image( sfg::Image::Create( close_image ) );
			btnhidewindow->SetImage( image );
		}
	}

	sfg::Button::Ptr btntogglespace( sfg::Button::Create( L"Box Spacing") );
	sfg::Button::Ptr btnloadstyle( sfg::Button::Create( L"Load theme") );
	sfg::Button::Ptr btntoggleculling( sfg::Button::Create( L"Toggle culling") );

	m_entry = sfg::Entry::Create( L"Type something!" );
	m_entry->SetRequisition( sf::Vector2f( 100.f, .0f ) );

	m_limit_check = sfg::CheckButton::Create( L"Limit to 4 chars" );
	m_limit_check->SetId( "limit_check" );

	sfg::Entry::Ptr password( sfg::Entry::Create() );
	password->HideText( '*' );

	// Layout.
	sfg::Box::Ptr boxtoolbar( sfg::Box::Create( sfg::Box::HORIZONTAL ) );
	boxtoolbar->SetSpacing( 5.f );
	boxtoolbar->Pack( btnaddbuttonh, false );
	boxtoolbar->Pack( btnaddbuttonv, false );
	boxtoolbar->Pack( m_titlebar_toggle, false );
	boxtoolbar->Pack( btnhidewindow, false );
	boxtoolbar->Pack( m_entry, true );
	boxtoolbar->Pack( m_limit_check, false );

	sfg::Frame::Ptr frame1( sfg::Frame::Create( L"Toolbar 1" ) );
	frame1->Add( boxtoolbar );

	sfg::Box::Ptr boxtoolbar2( sfg::Box::Create( sfg::Box::HORIZONTAL ) );
	boxtoolbar2->SetSpacing( 5.f );
	boxtoolbar2->Pack( btntogglespace, false );
	boxtoolbar2->Pack( btnloadstyle, false );
	boxtoolbar2->Pack( btntoggleculling, false );

	m_boxbuttonsh = sfg::Box::Create( sfg::Box::HORIZONTAL );
	m_boxbuttonsh->SetSpacing( 5.f );

	m_boxbuttonsv = sfg::Box::Create( sfg::Box::VERTICAL );
	m_boxbuttonsv->SetSpacing( 5.f );

	sfg::Entry::Ptr username_entry( sfg::Entry::Create() );
	username_entry->SetMaximumLength( 8 );

	m_progress = sfg::ProgressBar::Create( sfg::ProgressBar::HORIZONTAL );
	m_progress->SetRequisition( sf::Vector2f( 0.f, 20.f ) );

	m_progress_vert = sfg::ProgressBar::Create( sfg::ProgressBar::VERTICAL );
	m_progress_vert->SetRequisition( sf::Vector2f( 20.f, 0.f ) );

	sfg::Separator::Ptr separatorv( sfg::Separator::Create( sfg::Separator::VERTICAL ) );

	m_table = sfg::Table::Create();
	m_table->Attach( sfg::Label::Create( L"Please login using your username and password (span example)." ), sf::Rect<sf::Uint32>( 0, 0, 2, 1 ), sfg::Table::FILL, sfg::Table::FILL | sfg::Table::EXPAND );
	m_table->Attach( sfg::Label::Create( L"Username:" ), sf::Rect<sf::Uint32>( 0, 1, 1, 1 ), sfg::Table::FILL, sfg::Table::FILL );
	m_table->Attach( username_entry, sf::Rect<sf::Uint32>( 1, 1, 1, 1 ), sfg::Table::EXPAND | sfg::Table::FILL, sfg::Table::FILL );
	m_table->Attach( sfg::Label::Create( L"Password:" ), sf::Rect<sf::Uint32>( 0, 2, 1, 1 ), sfg::Table::FILL, sfg::Table::FILL );
	m_table->Attach( password, sf::Rect<sf::Uint32>( 1, 2, 1, 1 ), sfg::Table::FILL, sfg::Table::FILL );
	m_table->Attach( sfg::Button::Create( L"Login" ), sf::Rect<sf::Uint32>( 2, 1, 1, 2 ), sfg::Table::FILL, sfg::Table::FILL );
	m_table->Attach( separatorv, sf::Rect<sf::Uint32>( 3, 0, 1, 3 ), sfg::Table::FILL, sfg::Table::FILL );
	m_table->Attach( m_progress_vert, sf::Rect<sf::Uint32>( 4, 0, 1, 3 ), sfg::Table::FILL, sfg::Table::FILL );
	m_table->SetRowSpacings( 5.f );
	m_table->SetColumnSpacings( 5.f );

	m_scrolled_window_box = sfg::Box::Create( sfg::Box::VERTICAL );

	for( int i = 0; i < 7; i++ ) {
		sfg::Box::Ptr box = sfg::Box::Create( sfg::Box::HORIZONTAL );

		for( int j = 0; j < 20; j++ ) {
			box->Pack( sfg::Button::Create( L"One button among many" ), true );
		}

		m_scrolled_window_box->Pack( box, false );
	}

	m_scrolled_window = sfg::ScrolledWindow::Create();
	m_scrolled_window->SetRequisition( sf::Vector2f( .0f, 200.f ) );
	m_scrolled_window->SetScrollbarPolicy( sfg::ScrolledWindow::HORIZONTAL_AUTOMATIC | sfg::ScrolledWindow::VERTICAL_AUTOMATIC );
	m_scrolled_window->SetPlacement( sfg::ScrolledWindow::TOP_LEFT );
	m_scrolled_window->AddWithViewport( m_scrolled_window_box );

	sfg::Scrollbar::Ptr scrollbar( sfg::Scrollbar::Create() );
	scrollbar->SetRange( .0f, 100.f );

	m_scale = sfg::Scale::Create();
	m_scale->SetAdjustment( scrollbar->GetAdjustment() );
	m_scale->SetRequisition( sf::Vector2f( 100.f, .0f ) );
	boxtoolbar2->Pack( m_scale, false );

	sfg::Frame::Ptr frame2( sfg::Frame::Create( L"Toolbar 2" ) );
	frame2->Add( boxtoolbar2 );
	frame2->SetAlignment( sf::Vector2f( .8f, .0f ) );

	sfg::Separator::Ptr separatorh( sfg::Separator::Create( sfg::Separator::HORIZONTAL ) );

	sfg::Box::Ptr box_image( sfg::Box::Create( sfg::Box::HORIZONTAL ) );
	box_image->SetSpacing( 5.f );

	sfg::Fixed::Ptr fixed_container( sfg::Fixed::Create() );
	sfg::Button::Ptr fixed_button( sfg::Button::Create( L"I'm at (34,61)" ) );
	fixed_container->Put( fixed_button, sf::Vector2f( 34.f, 61.f ) );
	box_image->Pack( fixed_container, false );

	sf::Image sfgui_logo;
	sfg::Image::Ptr image = sfg::Image::Create();

	if( sfgui_logo.LoadFromFile("sfgui.png") ) {
		image->SetImage( sfgui_logo );
		box_image->Pack( image, false );
	}

	sfg::Box::Ptr spinner_box( sfg::Box::Create( sfg::Box::VERTICAL ) );

	m_spinner = sfg::Spinner::Create();
	m_spinner->SetRequisition( sf::Vector2f( 40.f, 40.f ) );
	m_spinner->Start();
	sfg::ToggleButton::Ptr spinner_toggle( sfg::ToggleButton::Create( L"Spin") );
	spinner_toggle->SetActive( true );
	spinner_box->SetSpacing( 5.f );
	spinner_box->Pack( m_spinner, false );
	spinner_box->Pack( spinner_toggle, false );

	box_image->Pack( spinner_box, false );

	sfg::Button::Ptr aligned_button( sfg::Button::Create( L"I'm way over here" ) );

	sfg::Alignment::Ptr alignment( sfg::Alignment::Create() );
	alignment->Add( aligned_button );
	box_image->Pack( alignment, true );
	alignment->SetAlignment( sf::Vector2f( 1.f, 1.f ) );
	alignment->SetScale( sf::Vector2f( 0.f, .5f ) );

	sfg::Box::Ptr boxmain( sfg::Box::Create( sfg::Box::VERTICAL ) );
	boxmain->SetSpacing( 5.f );
	boxmain->Pack( scrollbar, false );
	boxmain->Pack( m_progress, false );
	boxmain->Pack( frame1, false );
	boxmain->Pack( frame2, false );
	boxmain->Pack( m_boxbuttonsh, false );
	boxmain->Pack( m_boxbuttonsv, false );
	boxmain->Pack( box_image );
	boxmain->Pack( separatorh, false );
	boxmain->Pack( m_table, true );
	boxmain->Pack( m_scrolled_window );

	sfg::Notebook::Ptr notebook1( sfg::Notebook::Create() );
	sfg::Notebook::Ptr notebook2( sfg::Notebook::Create() );
	sfg::Notebook::Ptr notebook3( sfg::Notebook::Create() );
	sfg::Notebook::Ptr notebook4( sfg::Notebook::Create() );

	notebook1->SetTabPosition( sfg::Notebook::TOP );
	notebook1->SetMargin( 10.f );
	notebook2->SetTabPosition( sfg::Notebook::RIGHT );
	notebook3->SetTabPosition( sfg::Notebook::BOTTOM );
	notebook4->SetTabPosition( sfg::Notebook::LEFT );

	sfg::Box::Ptr vertigo_box( sfg::Box::Create( sfg::Box::HORIZONTAL ) );
	sfg::Button::Ptr vertigo_button( sfg::Button::Create( L"Vertigo" ) );
	vertigo_box->Pack( vertigo_button, true, true );

	notebook1->AppendPage( boxmain, sfg::Label::Create( "Page Name Here" ) );
	notebook1->AppendPage( notebook2, sfg::Label::Create( "Another Page" ) );
	notebook2->AppendPage( notebook3, sfg::Label::Create( "Yet Another Page" ) );
	notebook2->AppendPage( sfg::Label::Create( L"" ), sfg::Label::Create( "Dummy Page" ) );
	notebook3->AppendPage( notebook4, sfg::Label::Create( "And Another Page" ) );
	notebook3->AppendPage( sfg::Label::Create( L"" ), sfg::Label::Create( "Dummy Page" ) );
	notebook4->AppendPage( vertigo_box, sfg::Label::Create( "And The Last Page" ) );
	notebook4->AppendPage( sfg::Label::Create( L"" ), sfg::Label::Create( "Dummy Page" ) );

	m_wndmain->Add( notebook1 );

	// Signals.
	btnaddbuttonh->OnClick.Connect( &SampleApp::OnAddButtonHClick, this );
	btnaddbuttonv->OnClick.Connect( &SampleApp::OnAddButtonVClick, this );
	m_titlebar_toggle->OnClick.Connect( &SampleApp::OnToggleTitlebarClick, this );
	btnhidewindow->OnClick.Connect( &SampleApp::OnHideWindowClicked, this );
	btntogglespace->OnClick.Connect( &SampleApp::OnToggleSpaceClick, this );
	m_limit_check->OnToggle.Connect( &SampleApp::OnLimitCharsToggle, this );
	btnloadstyle->OnClick.Connect( &SampleApp::OnLoadThemeClick, this );
	btntoggleculling->OnClick.Connect( &SampleApp::OnToggleCullingClick, this );
	m_scale->GetAdjustment()->OnChange.Connect( &SampleApp::OnAdjustmentChange, this );
	spinner_toggle->OnClick.Connect( &SampleApp::OnToggleSpinner, this );

	m_wndmain->SetPosition( sf::Vector2f( 100.f, 100.f ) );

	// Another window
	sfg::Window::Ptr second_window( sfg::Window::Create() );
	second_window->SetId( "second_window" );
	second_window->SetMargin( 10.f );
	second_window->SetTitle( "Another window" );
	sfg::Box::Ptr box( sfg::Box::Create( sfg::Box::VERTICAL, 5.f ) );
	box->Pack( sfg::Label::Create( "Aliquam sed pretium lacus." ), false );
	box->Pack( sfg::Label::Create( "Nullam placerat mauris vel nulla sagittis pellentesque." ), false );
	box->Pack( sfg::Label::Create( "Suspendisse in justo dui." ), false );
	box->Pack( sfg::Label::Create( "Ut dolor massa, gravida eu facilisis convallis, convallis sed odio." ), false );
	box->Pack( sfg::Label::Create( "Nunc placerat consequat vehicula." ), false );
	second_window->Add( box );
	second_window->SetPosition( sf::Vector2f( 10.f, 10.f ) );
	second_window->SetId( "second_window" );
	m_desktop.Add( second_window );

	// Add window to desktop
	m_desktop.Add( m_wndmain );

	// Play around with resource manager.
	sf::Font my_font;
	my_font.LoadFromFile( "data/linden_hill.otf" );
	m_desktop.GetEngine().GetResourceManager().AddFont( "custom_font", my_font, false ); // false -> do not manage!

	// Set properties.
	m_desktop.SetProperty( "Button#close:Normal", "Color", sf::Color::Yellow );
	m_desktop.SetProperty( "Button#close", "FontName", "data/linden_hill.otf" );
	m_desktop.SetProperty( "Button#close", "FontSize", 15.f );
	m_desktop.SetProperty( "Window#second_window > Box > Label", "FontName", "custom_font" );
	m_desktop.SetProperty( "Window#second_window > Box > Label", "FontSize", 18.f );

	m_fps_counter = 0;
	m_fps_clock.Reset();

	sfg::CullingTarget culling_target( m_window );

	while( m_window.IsOpened() ) {
		while( m_window.PollEvent( event ) ) {
			if( event.Type == sf::Event::Closed ) {
				m_window.Close();
			}
			else if( event.Type == sf::Event::Resized ) {
				m_desktop.UpdateViewRect( sf::FloatRect( 0.f, 0.f, static_cast<float>( event.Size.Width ), static_cast<float>( event.Size.Height ) ) );
			}

			m_desktop.HandleEvent( event );
		}

		m_window.Clear( sf::Color( 80, 80, 80 ) );
		culling_target.Cull( m_cull );
		m_window.Draw( m_background_sprite );
		m_desktop.Expose( culling_target );
		m_window.Display();

		if( m_fps_clock.GetElapsedTime() >= 1000 ) {
			m_fps_clock.Reset();

			std::stringstream sstr;
			sstr << "SFGUI test -- FPS: " << m_fps_counter;
			if( m_cull ) {
				sstr << " -- Cull K/D: " << culling_target.GetCount().first
				 << "/" << culling_target.GetCount().second;
			}
			m_window.SetTitle( sstr.str() );

			m_fps_counter = 0;
			culling_target.ResetCount();
		}

		++m_fps_counter;
	}
}

void SampleApp::OnAddButtonHClick() {
	sfg::Button::Ptr button( sfg::Button::Create( L"New ->" ) );

	Ouchy::Ptr ouchy( new Ouchy( button ) );
	Ouchy::m_ouchies.push_back( ouchy );

	button->OnClick.Connect( &Ouchy::DoOuch, ouchy.get() );

	m_boxbuttonsh->Pack( button, true );
}

void SampleApp::OnAddButtonVClick() {
	sfg::Button::Ptr button( sfg::Button::Create( L"<- New" ) );

	Ouchy::Ptr ouchy( new Ouchy( button ) );
	Ouchy::m_ouchies.push_back( ouchy );

	button->OnClick.Connect( &Ouchy::DoOuch, ouchy.get() );

	m_boxbuttonsv->Pack( button, false );
}

void SampleApp::OnToggleTitlebarClick() {
	m_wndmain->SetStyle( m_wndmain->GetStyle() ^ sfg::Window::Titlebar );
}

void SampleApp::OnHideWindowClicked() {
	m_wndmain->Show( !m_wndmain->IsVisible() );
}

void SampleApp::OnToggleSpaceClick() {
	if( m_scrolled_window_box->GetSpacing() > .0f ) {
		m_scrolled_window_box->SetSpacing( .0f );
		m_scrolled_window_box->SetMargin( .0f );
	}
	else {
		m_scrolled_window_box->SetSpacing( 40.f );
		m_scrolled_window_box->SetMargin( 40.f );
	}
}

void SampleApp::OnLimitCharsToggle() {
	if( m_limit_check->IsActive() ) {
		m_entry->SetMaximumLength( 4 );
	}
	else {
		m_entry->SetMaximumLength( 0 );
	}
}

void SampleApp::OnLoadThemeClick() {
	m_desktop.LoadThemeFromFile( "data/example.theme" );
}

void SampleApp::OnToggleCullingClick() {
	m_cull = !m_cull;
}

void SampleApp::OnAdjustmentChange() {
	m_progress->SetFraction( m_scale->GetValue() / 100.f );
	m_progress_vert->SetFraction( m_scale->GetValue() / 100.f );
}

void SampleApp::OnToggleSpinner() {
	if( !m_spinner->Started() ) {
		m_spinner->Start();
	}
	else {
		m_spinner->Stop();
	}
}

int main() {
	SampleApp app;
	app.Run();
	return 0;
}
