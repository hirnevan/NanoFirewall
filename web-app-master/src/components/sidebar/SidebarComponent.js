import React from 'react';
import { createUseStyles, useTheme } from 'react-jss';
import { useHistory } from 'react-router-dom';
import SLUGS from 'resources/slugs';
import { convertSlugToUrl } from 'resources/utilities';
import LogoComponent from './LogoComponent';
import Menu from './MenuComponent';
import MenuItem from './MenuItemComponent';
import iconAnchor from '../../assets/icons/icon-anchor';
import iconHome from '../../assets/icons/icon-home';
import iconBook from '../../assets/icons/icon-book';
import iconMonitor from '../../assets/icons/icon-monitor';
import iconGlobe from '../../assets/icons/icon-globe';
import iconHardDrive from '../../assets/icons/icon-hard-drive';
import iconClock from '../../assets/icons/icon-clock';
import iconLayers from '../../assets/icons/icon-layers';
import iconCompass from '../../assets/icons/icon-compass';
import iconShield from '../../assets/icons/icon-shield';
import iconUsers from '../../assets/icons/icon-users';
import iconTool from '../../assets/icons/icon-tool';
import iconArchive from '../../assets/icons/icon-archive';
import iconHelp from '../../assets/icons/icon-help';

const useStyles = createUseStyles({
    separator: {
        borderTop: ({ theme }) => `1px solid ${theme.color.lightGrayishBlue}`,
        marginTop: 16,
        marginBottom: 16,
        opacity: 0.06
    }
});

function SidebarComponent() {
    const { push } = useHistory();
    const theme = useTheme();
    const classes = useStyles({ theme });
    const isMobile = window.innerWidth <= 1080;

    function onClick(slug, parameters = {}) {
        push(convertSlugToUrl(slug, parameters));
    }

    return (
        <Menu isMobile={isMobile}>
            <div style={{ paddingTop: 30, paddingBottom: 30 }}>
                <LogoComponent />
            </div>
            <MenuItem
                id={SLUGS.home}
                title='Home'
                icon={iconHome}
                onClick={() => onClick(SLUGS.home)}
            />
            <MenuItem
                id={SLUGS.contacts}
                title='Manage Devices'
                icon={iconUsers}
                onClick={() => onClick(SLUGS.management)}
            />
            <MenuItem
                id={SLUGS.rules}
                title='Manage Rules'
                icon={iconBook}
            >
                <MenuItem
                    id={SLUGS.rulesIP}
                    title='IP filters'
                    level={2}
                    icon={iconMonitor}
                    onClick={() => onClick(SLUGS.rulesIP)}
                />
                <MenuItem
                    id={SLUGS.rulesDNS}
                    title='Website filters'
                    level={2}
                    icon={iconGlobe}
                    onClick={() => onClick(SLUGS.rulesDNS)}
                />
                <MenuItem
                    id={SLUGS.rulesMAC}
                    title='MAC filters'
                    level={2}
                    icon={iconHardDrive}
                    onClick={() => onClick(SLUGS.rulesMAC)}
                />
                <MenuItem
                    id={SLUGS.time}
                    title='Time Filters'
                    level={2}
                    icon={iconClock}
                    onClick={() => onClick(SLUGS.time)}
                />
                <MenuItem
                    id={SLUGS.rulesPort}
                    title='Port filters'
                    level={2}
                    icon={iconAnchor}
                    onClick={() => onClick(SLUGS.rulesPort)}
                />
                <MenuItem
                    id={SLUGS.rulesProtocol}
                    title='Protocol filters'
                    level={2}
                    icon={iconLayers}
                    onClick={() => onClick(SLUGS.rulesProtocol)}
                />
                <MenuItem
                    id={SLUGS.rulesSmart}
                    title='Smart filters'
                    level={2}
                    icon={iconShield}
                    onClick={() => onClick(SLUGS.rulesSmart)}
                />
                <MenuItem
                    id={SLUGS.rulesLocation}
                    title='Location filters'
                    level={2}
                    icon={iconCompass}
                    onClick={() => onClick(SLUGS.rulesLocation)}
                />
            </MenuItem>
            <div className={classes.separator}></div>
            <MenuItem
                id={SLUGS.fwsettings}
                title='Firewall Settings'
                icon={iconTool}
                onClick={() => onClick(SLUGS.fwsettings)}
            />
            <MenuItem
                id={SLUGS.presets}
                title='Firewall Presets'
                icon={iconArchive}
                onClick={() => onClick(SLUGS.presets)}
            />
            <MenuItem
                id={SLUGS.help}
                title='Help and FAQ'
                icon={iconHelp}
                onClick={() => onClick(SLUGS.help)}
            />
        </Menu>
    );
}

export default SidebarComponent;
