#include "../include/input_component.hpp"

struct UserInput::Private
{
  static Component create_event (ScreenInteractive &screen, Component &renderer)
  {
    return CatchEvent (renderer, [&] (Event const &event)
    {
      if (event == Event::Character ('\n'))
        {
          screen.ExitLoopClosure () ();
          return true;
        }
      return false;
    });
  }
   static Color::Palette16 which_color(Changeset &changeset , error_type err)
   {
    return changeset.has_error (err) ? Color::Red : Color::White;
   }

   static Element render_more_errors(Changeset &changeset)
   {
     if (changeset.m_errors.size() > 1)
       return text("More errors: -> " + changeset.other_errors()) | color(Color::Red);
     else
       return text("");
   };

};

UserInput::UserInput(Changeset &changeset, const std::string &filename, const std::string &path)
{
  changeset.fill_empty_fields ();

  Component client_input                = Input(&changeset.m_fields[client], "Missing Client Name");
  Component project_input               = Input(&changeset.m_fields[project], "Missing Project Name");
  Component recording_name_input        = Input(&changeset.m_fields[name], "Missing Recording Name");
  Component recording_descriptor_input  = Input(&changeset.m_fields[descriptor], "Missing Descriptor Name");
  Component date_input                  = Input(&changeset.m_fields[date], "Missing Date");
  Component key_input                   = Input(&changeset.m_fields[key], "Missing Key");
  Component bpm_input                   = Input(&changeset.m_fields[bpm], "Missing BPM");
  Component ts_input                    = Input(&changeset.m_fields[ts], "Missing time Signature");
  Component composer_input              = Input(&changeset.m_fields[composer], "Missing Composer Initials");
  Component talent_input                = Input(&changeset.m_fields[external_talent], "Missing External Talent or NXT");


  Component project_container = Container::Vertical({
                                                     client_input,
                                                     project_input,
                                                     recording_name_input,
                                                     recording_descriptor_input,
                                                     date_input,
                                                     key_input,
                                                     bpm_input,
                                                     ts_input,
                                                     composer_input,
                                                     talent_input
                                                     });

  Component renderer = Renderer(project_container, [&] {
    return  vbox({
               text(filename) | color(Color::Blue),
               text(changeset.first_error()) | color(Color::Red),
               Private::render_more_errors (changeset),
               separator(),
               text("Found at -> " + path +  ". Press 'o' to open in Finder.") | color(Color::Cyan),
               separator(),
               hbox(text(" Client:"), client_input->Render()  | color(Private::which_color (changeset, bad_client))),
               hbox(text(" Project: "), project_input->Render() | color(Private::which_color (changeset, bad_project)) ),
               hbox(text(" Recording: "), recording_name_input->Render() | color(Private::which_color (changeset, bad_name))),
               hbox(text(" Descriptor: "), recording_descriptor_input->Render() | color(Private::which_color (changeset, bad_descriptor))),
               hbox(text(" Date: "), date_input->Render() | color(Private::which_color (changeset, bad_date))),
               hbox(text(" Key: "), key_input->Render() | color(Private::which_color (changeset, bad_key))),
               hbox(text(" BPM: "), bpm_input->Render() | color(Private::which_color (changeset, bad_bpm))),
               hbox(text(" Time Signature: "), ts_input->Render() | color(Private::which_color (changeset, bad_ts))),
               hbox(text(" Composer: "), composer_input->Render() | color(Private::which_color (changeset, bad_composer))),
               hbox(text(" Talent: "), talent_input->Render() | color(Private::which_color (changeset, bad_external_talent))),
           }) | border;
  });

  auto screen                  = ScreenInteractive::TerminalOutput();
  auto event                   = Private::create_event(screen, renderer);
  screen.Loop(event);
};