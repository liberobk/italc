/*
 * ItalcCore.h - definitions for iTALC Core
 *
 * Copyright (c) 2006-2017 Tobias Doerffel <tobydox/at/users/dot/sf/dot/net>
 *
 * This file is part of iTALC - http://italc.sourceforge.net
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public
 * License as published by the Free Software Foundation; either
 * version 2 of the License, or (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * General Public License for more details.
 *
 * You should have received a copy of the GNU General Public
 * License along with this program (see COPYING); if not, write to the
 * Free Software Foundation, Inc., 59 Temple Place - Suite 330,
 * Boston, MA 02111-1307, USA.
 *
 */

#ifndef ITALC_CORE_H
#define ITALC_CORE_H

#include <italcconfig.h>

#include <QtCore/QPair>
#include <QtCore/QString>
#include <QtCore/QVariant>

#if defined(BUILD_ITALC_CORE_LIBRARY)
#  define ITALC_CORE_EXPORT Q_DECL_EXPORT
#else
#  define ITALC_CORE_EXPORT Q_DECL_IMPORT
#endif

class AuthenticationCredentials;
class ItalcConfiguration;
class SocketDevice;
class QWidget;

namespace ItalcCore
{
	void ITALC_CORE_EXPORT setupApplicationParameters();
	bool ITALC_CORE_EXPORT init();
	bool ITALC_CORE_EXPORT initAuthentication( int credentialTypes );
	void ITALC_CORE_EXPORT destroy();

	QString ITALC_CORE_EXPORT applicationName();
	void ITALC_CORE_EXPORT enforceBranding( QWidget* topLevelWidget );

	extern ITALC_CORE_EXPORT ItalcConfiguration *config;
	extern ITALC_CORE_EXPORT AuthenticationCredentials *authenticationCredentials;

	typedef QString Command;
	typedef QMap<QString, QVariant> CommandArgs;
	typedef QList<QPair<ItalcCore::Command, ItalcCore::CommandArgs> >
								CommandList;

	// static commands
	extern ITALC_CORE_EXPORT const Command GetUserInformation;
	extern ITALC_CORE_EXPORT const Command UserInformation;
	extern ITALC_CORE_EXPORT const Command LockInput;
	extern ITALC_CORE_EXPORT const Command UnlockInput;
	extern ITALC_CORE_EXPORT const Command LogonUserCmd;
	extern ITALC_CORE_EXPORT const Command LogoutUser;
	extern ITALC_CORE_EXPORT const Command AccessDialog;
	extern ITALC_CORE_EXPORT const Command ExecCmds;
	extern ITALC_CORE_EXPORT const Command DisableLocalInputs;
	extern ITALC_CORE_EXPORT const Command SetRole;
	extern ITALC_CORE_EXPORT const Command ReportSlaveStateFlags;

	class ITALC_CORE_EXPORT Msg
	{
	public:
		Msg( SocketDevice *sockDev, const Command &cmd = Command() ) :
			m_socketDevice( sockDev ),
			m_cmd( cmd )
		{
		}

		Msg( const Command &cmd ) :
			m_socketDevice( NULL ),
			m_cmd( cmd )
		{
		}

		const Command &cmd() const
		{
			return m_cmd;
		}

		const CommandArgs &args() const
		{
			return m_args;
		}

		void setSocketDevice( SocketDevice *sockDev )
		{
			m_socketDevice = sockDev;
		}

		Msg &addArg( const QString &key, const QString &value )
		{
			m_args[key.toLower()] = value;
			return *this;
		}

		Msg &addArg( const QString &key, const int value )
		{
			m_args[key.toLower()] = QString::number( value );
			return *this;
		}

		QString arg( const QString &key ) const
		{
			return m_args[key.toLower()].toString();
		}

		bool send();
		Msg &receive();


	private:
		SocketDevice *m_socketDevice;

		Command m_cmd;
		CommandArgs m_args;

	} ;

	enum UserRoles
	{
		RoleNone,
		RoleTeacher,
		RoleAdmin,
		RoleSupporter,
		RoleOther,
		RoleCount
	} ;
	typedef UserRoles UserRole;

	enum SlaveStateFlags
	{
		AccessDialogRunning 	= 1,
		InputLockRunning 		= 16,
	} ;

	QString userRoleName( UserRole role );


	extern ITALC_CORE_EXPORT int serverPort;
	extern ITALC_CORE_EXPORT UserRoles role;

}

#endif